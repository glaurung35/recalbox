// SPDX-License-Identifier: GPL-2.0-only
/*
 *  PCA953x 4/8/16/24/40 bit I/O ports
 *
 *  Copyright (C) 2005 Ben Gardner <bgardner@wabtec.com>
 *  Copyright (C) 2007 Marvell International Ltd.
 *  Copyright (C) 2023 digitalLumberjack at recalbox.com.
 *
 *  Derived from drivers/gpio/gpio-pca953x.c
 */

#include <linux/acpi.h>
#include <linux/bitmap.h>
#include <linux/gpio/driver.h>
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/platform_data/pca953x.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/input.h>
#include <linux/timekeeping.h>

#include <asm/unaligned.h>

#define PCA953X_INPUT    0x00
#define PCA953X_OUTPUT    0x01
#define PCA953X_INVERT    0x02
#define PCA953X_DIRECTION  0x03

#define REG_ADDR_MASK    GENMASK(5, 0)
#define REG_ADDR_EXT    BIT(6)
#define REG_ADDR_AI    BIT(7)

#define PCAL953X_OUT_STRENGTH  0x20
#define PCAL953X_IN_LATCH  0x22
#define PCAL953X_PULL_EN  0x23
#define PCAL953X_PULL_SEL  0x24
#define PCAL953X_INT_MASK  0x25
#define PCAL953X_INT_STAT  0x26
#define PCAL953X_OUT_CONF  0x27

#define PCA_GPIO_MASK    GENMASK(7, 0)

#define PCAL_GPIO_MASK    GENMASK(4, 0)
#define PCAL_PINCTRL_MASK  GENMASK(6, 5)

#define PCA_INT      BIT(8)
#define PCA_PCAL    BIT(9)
#define PCA_LATCH_INT    (PCA_PCAL | PCA_INT)
#define PCA953X_TYPE    BIT(12)
#define PCA_TYPE_MASK    GENMASK(15, 12)

#define PCA_CHIP_TYPE(x)  ((x) & PCA_TYPE_MASK)

#define DEBUG 1

static const struct i2c_device_id pca953x_id[] = {
    {"recalboxrgbjamma", 16 | PCA953X_TYPE | PCA_INT,},
    {}
};
MODULE_DEVICE_TABLE(i2c, pca953x_id
);

// Interupts
#include <linux/dmi.h>

static const struct acpi_gpio_params pca953x_irq_gpios = {0, 0, true};

static const struct acpi_gpio_mapping pca953x_acpi_irq_gpios[] = {
    {"irq-gpios", &pca953x_irq_gpios, 1, ACPI_GPIO_QUIRK_ABSOLUTE_NUMBER},
    {}
};

static int pca953x_acpi_get_irq(struct device *dev) {
  int ret;

  ret = devm_acpi_dev_add_driver_gpios(dev, pca953x_acpi_irq_gpios);
  if (ret)
    dev_warn(dev, "can't add GPIO ACPI mapping\n");

  ret = acpi_dev_gpio_irq_get_by(ACPI_COMPANION(dev), "irq-gpios", 0);
  if (ret < 0) {
    dev_info(dev, "could not get acpi gpio by irq-gpios\n");
    return ret;
  }

  dev_info(dev, "ACPI interrupt quirk (IRQ %d)\n", ret);
  return ret;
}

static const struct acpi_device_id pca953x_acpi_ids[] = {
    {"INT3491", 16 | PCA953X_TYPE | PCA_LATCH_INT,},
    {}
};

#define MAX_BANK 5
#define BANK_SZ 8
#define MAX_LINE  (MAX_BANK * BANK_SZ)

#define NBANK(chip) DIV_ROUND_UP(chip->gpio_chip.ngpio, BANK_SZ)

struct pca953x_reg_config {
  int direction;
  int output;
  int input;
  int invert;
};

static const struct pca953x_reg_config pca953x_regs = {
    .direction = PCA953X_DIRECTION,
    .output = PCA953X_OUTPUT,
    .input = PCA953X_INPUT,
    .invert = PCA953X_INVERT,
};

struct pca953x_chip {
  unsigned gpio_start;
  struct mutex i2c_lock;
  struct regmap *regmap;

  struct mutex irq_lock;
  DECLARE_BITMAP(irq_mask, MAX_LINE);
  DECLARE_BITMAP(irq_stat, MAX_LINE);
  DECLARE_BITMAP(irq_trig_raise, MAX_LINE);
  DECLARE_BITMAP(irq_trig_fall, MAX_LINE);
  struct irq_chip irq_chip;

  atomic_t wakeup_path;

  struct i2c_client *client;
  struct gpio_chip gpio_chip;
  const char *const *names;
  unsigned long driver_data;
  struct regulator *regulator;

  const struct pca953x_reg_config *regs;
};

static int pca953x_bank_shift(struct pca953x_chip *chip) {
  return fls((chip->gpio_chip.ngpio - 1) / BANK_SZ);
}

#define PCA953x_BANK_INPUT  BIT(0)
#define PCA953x_BANK_OUTPUT  BIT(1)
#define PCA953x_BANK_POLARITY  BIT(2)
#define PCA953x_BANK_CONFIG  BIT(3)

#define PCAL9xxx_BANK_IN_LATCH  BIT(8 + 2)
#define PCAL9xxx_BANK_PULL_EN  BIT(8 + 3)
#define PCAL9xxx_BANK_PULL_SEL  BIT(8 + 4)
#define PCAL9xxx_BANK_IRQ_MASK  BIT(8 + 5)
#define PCAL9xxx_BANK_IRQ_STAT  BIT(8 + 6)

/*
 * We care about the following registers:
 * - Standard set, below 0x40, each port can be replicated up to 8 times
 *   - PCA953x standard
 *     Input port			0x00 + 0 * bank_size	R
 *     Output port			0x00 + 1 * bank_size	RW
 *     Polarity Inversion port		0x00 + 2 * bank_size	RW
 *     Configuration port		0x00 + 3 * bank_size	RW
 *   - PCA957x with mixed up registers
 *     Input port			0x00 + 0 * bank_size	R
 *     Polarity Inversion port		0x00 + 1 * bank_size	RW
 *     Bus hold port			0x00 + 2 * bank_size	RW
 *     Configuration port		0x00 + 4 * bank_size	RW
 *     Output port			0x00 + 5 * bank_size	RW
 *
 * - Extended set, above 0x40, often chip specific.
 *   - PCAL6524/PCAL9555A with custom PCAL IRQ handling:
 *     Input latch register		0x40 + 2 * bank_size	RW
 *     Pull-up/pull-down enable reg	0x40 + 3 * bank_size    RW
 *     Pull-up/pull-down select reg	0x40 + 4 * bank_size    RW
 *     Interrupt mask register		0x40 + 5 * bank_size	RW
 *     Interrupt status register	0x40 + 6 * bank_size	R
 *
 * - Registers with bit 0x80 set, the AI bit
 *   The bit is cleared and the registers fall into one of the
 *   categories above.
 */

static bool pca953x_check_register(struct pca953x_chip *chip, unsigned int reg,
                                   u32 checkbank) {
  int bank_shift = pca953x_bank_shift(chip);
  int bank = (reg & REG_ADDR_MASK) >> bank_shift;
  int offset = reg & (BIT(bank_shift) - 1);

  /* Special PCAL extended register check. */
  if (reg & REG_ADDR_EXT) {
    if (!(chip->driver_data & PCA_PCAL))
      return false;
    bank += 8;
  }

  /* Register is not in the matching bank. */
  if (!(BIT(bank) & checkbank))
    return false;

  /* Register is not within allowed range of bank. */
  if (offset >= NBANK(chip))
    return false;

  return true;
}

static bool pca953x_readable_register(struct device *dev, unsigned int reg) {
  struct pca953x_chip *chip = dev_get_drvdata(dev);
  u32 bank;

  bank = PCA953x_BANK_INPUT | PCA953x_BANK_OUTPUT |
         PCA953x_BANK_POLARITY | PCA953x_BANK_CONFIG;

  if (chip->driver_data & PCA_PCAL) {
    bank |= PCAL9xxx_BANK_IN_LATCH | PCAL9xxx_BANK_PULL_EN |
            PCAL9xxx_BANK_PULL_SEL | PCAL9xxx_BANK_IRQ_MASK |
            PCAL9xxx_BANK_IRQ_STAT;
  }

  return pca953x_check_register(chip, reg, bank);
}

static bool pca953x_writeable_register(struct device *dev, unsigned int reg) {
  struct pca953x_chip *chip = dev_get_drvdata(dev);
  u32 bank;

  bank = PCA953x_BANK_OUTPUT | PCA953x_BANK_POLARITY |
         PCA953x_BANK_CONFIG;

  if (chip->driver_data & PCA_PCAL)
    bank |= PCAL9xxx_BANK_IN_LATCH | PCAL9xxx_BANK_PULL_EN |
            PCAL9xxx_BANK_PULL_SEL | PCAL9xxx_BANK_IRQ_MASK;

  return pca953x_check_register(chip, reg, bank);
}

static bool pca953x_volatile_register(struct device *dev, unsigned int reg) {
  struct pca953x_chip *chip = dev_get_drvdata(dev);
  u32 bank;

  bank = PCA953x_BANK_INPUT;

  if (chip->driver_data & PCA_PCAL)
    bank |= PCAL9xxx_BANK_IRQ_STAT;

  return pca953x_check_register(chip, reg, bank);
}

static const struct regmap_config pca953x_i2c_regmap = {
    .reg_bits = 8,
    .val_bits = 8,

    .use_single_read = true,
    .use_single_write = true,

    .readable_reg = pca953x_readable_register,
    .writeable_reg = pca953x_writeable_register,
    .volatile_reg = pca953x_volatile_register,

    .disable_locking = true,
    .cache_type = REGCACHE_RBTREE,
    .max_register = 0x7f,
};


static u8 pca953x_recalc_addr(struct pca953x_chip *chip, int reg, int off) {
  int bank_shift = pca953x_bank_shift(chip);
  int addr = (reg & PCAL_GPIO_MASK) << bank_shift;
  int pinctrl = (reg & PCAL_PINCTRL_MASK) << 1;
  u8 regaddr = pinctrl | addr | (off / BANK_SZ);

  return regaddr;
}

static int pca953x_write_regs(struct pca953x_chip *chip, int reg, unsigned long *val) {
  u8 regaddr = pca953x_recalc_addr(chip, reg, 0);
  u8 value[MAX_BANK];
  int i, ret;

  for (i = 0; i < NBANK(chip); i++)
    value[i] = bitmap_get_value8(val, i * BANK_SZ);

  ret = regmap_bulk_write(chip->regmap, regaddr, value, NBANK(chip));
  if (ret < 0) {
    dev_err(&chip->client->dev, "failed writing register\n");
    return ret;
  }

  return 0;
}

static int pca953x_read_regs(struct pca953x_chip *chip, int reg, unsigned long *val) {
  u8 regaddr = pca953x_recalc_addr(chip, reg, 0);
  u8 value[MAX_BANK];
  int i, ret;

  ret = regmap_bulk_read(chip->regmap, regaddr, value, NBANK(chip));
  if (ret < 0) {
    dev_err(&chip->client->dev, "failed reading register\n");
    return ret;
  }

  for (i = 0; i < NBANK(chip); i++)
    bitmap_set_value8(val, value[i], i * BANK_SZ);

  return 0;
}

static int pca953x_gpio_direction_input(struct gpio_chip *gc, unsigned off) {
  struct pca953x_chip *chip = gpiochip_get_data(gc);
  u8 dirreg = pca953x_recalc_addr(chip, chip->regs->direction, off);
  u8 bit = BIT(off % BANK_SZ);
  int ret;

  mutex_lock(&chip->i2c_lock);
  ret = regmap_write_bits(chip->regmap, dirreg, bit, bit);
  mutex_unlock(&chip->i2c_lock);
  return ret;
}

static int pca953x_gpio_direction_output(struct gpio_chip *gc,
                                         unsigned off, int val) {
  struct pca953x_chip *chip = gpiochip_get_data(gc);
  u8 dirreg = pca953x_recalc_addr(chip, chip->regs->direction, off);
  u8 outreg = pca953x_recalc_addr(chip, chip->regs->output, off);
  u8 bit = BIT(off % BANK_SZ);
  int ret;

  mutex_lock(&chip->i2c_lock);
  /* set output level */
  ret = regmap_write_bits(chip->regmap, outreg, bit, val ? bit : 0);
  if (ret)
    goto exit;

  /* then direction */
  ret = regmap_write_bits(chip->regmap, dirreg, bit, 0);
  exit:
  mutex_unlock(&chip->i2c_lock);
  return ret;
}

static int pca953x_gpio_get_value(struct gpio_chip *gc, unsigned off) {
  struct pca953x_chip *chip = gpiochip_get_data(gc);
  u8 inreg = pca953x_recalc_addr(chip, chip->regs->input, off);
  u8 bit = BIT(off % BANK_SZ);
  u32 reg_val;
  int ret;

  mutex_lock(&chip->i2c_lock);
  ret = regmap_read(chip->regmap, inreg, &reg_val);
  mutex_unlock(&chip->i2c_lock);
  if (ret < 0)
    return ret;

  return !!(reg_val & bit);
}

static void pca953x_gpio_set_value(struct gpio_chip *gc, unsigned off, int val) {
  struct pca953x_chip *chip = gpiochip_get_data(gc);
  u8 outreg = pca953x_recalc_addr(chip, chip->regs->output, off);
  u8 bit = BIT(off % BANK_SZ);

  mutex_lock(&chip->i2c_lock);
  regmap_write_bits(chip->regmap, outreg, bit, val ? bit : 0);
  mutex_unlock(&chip->i2c_lock);
}

static int pca953x_gpio_get_direction(struct gpio_chip *gc, unsigned off) {
  struct pca953x_chip *chip = gpiochip_get_data(gc);
  u8 dirreg = pca953x_recalc_addr(chip, chip->regs->direction, off);
  u8 bit = BIT(off % BANK_SZ);
  u32 reg_val;
  int ret;

  mutex_lock(&chip->i2c_lock);
  ret = regmap_read(chip->regmap, dirreg, &reg_val);
  mutex_unlock(&chip->i2c_lock);
  if (ret < 0)
    return ret;

  if (reg_val & bit)
    return GPIO_LINE_DIRECTION_IN;

  return GPIO_LINE_DIRECTION_OUT;
}

static int pca953x_gpio_get_multiple(struct gpio_chip *gc,
                                     unsigned long *mask, unsigned long *bits) {
  struct pca953x_chip *chip = gpiochip_get_data(gc);
  DECLARE_BITMAP(reg_val, MAX_LINE);
  int ret;

  mutex_lock(&chip->i2c_lock);
  ret = pca953x_read_regs(chip, chip->regs->input, reg_val);
  mutex_unlock(&chip->i2c_lock);
  if (ret)
    return ret;

  bitmap_replace(bits, bits, reg_val, mask, gc->ngpio);
  return 0;
}

static void pca953x_gpio_set_multiple(struct gpio_chip *gc,
                                      unsigned long *mask, unsigned long *bits) {
  struct pca953x_chip *chip = gpiochip_get_data(gc);
  DECLARE_BITMAP(reg_val, MAX_LINE);
  int ret;

  mutex_lock(&chip->i2c_lock);
  ret = pca953x_read_regs(chip, chip->regs->output, reg_val);
  if (ret)
    goto exit;

  bitmap_replace(reg_val, reg_val, bits, mask, gc->ngpio);

  pca953x_write_regs(chip, chip->regs->output, reg_val);
  exit:
  mutex_unlock(&chip->i2c_lock);
}

static int pca953x_gpio_set_pull_up_down(struct pca953x_chip *chip,
                                         unsigned int offset,
                                         unsigned long config) {
  u8 pull_en_reg = pca953x_recalc_addr(chip, PCAL953X_PULL_EN, offset);
  u8 pull_sel_reg = pca953x_recalc_addr(chip, PCAL953X_PULL_SEL, offset);
  u8 bit = BIT(offset % BANK_SZ);
  int ret;

  /*
   * pull-up/pull-down configuration requires PCAL extended
   * registers
   */
  if (!(chip->driver_data & PCA_PCAL))
    return -ENOTSUPP;

  mutex_lock(&chip->i2c_lock);

  /* Configure pull-up/pull-down */
  if (config == PIN_CONFIG_BIAS_PULL_UP)
    ret = regmap_write_bits(chip->regmap, pull_sel_reg, bit, bit);
  else if (config == PIN_CONFIG_BIAS_PULL_DOWN)
    ret = regmap_write_bits(chip->regmap, pull_sel_reg, bit, 0);
  else
    ret = 0;
  if (ret)
    goto exit;

  /* Disable/Enable pull-up/pull-down */
  if (config == PIN_CONFIG_BIAS_DISABLE)
    ret = regmap_write_bits(chip->regmap, pull_en_reg, bit, 0);
  else
    ret = regmap_write_bits(chip->regmap, pull_en_reg, bit, bit);

  exit:
  mutex_unlock(&chip->i2c_lock);
  return ret;
}

static int pca953x_gpio_set_config(struct gpio_chip *gc, unsigned int offset,
                                   unsigned long config) {
  struct pca953x_chip *chip = gpiochip_get_data(gc);

  switch (pinconf_to_config_param(config)) {
    case PIN_CONFIG_BIAS_PULL_UP:
    case PIN_CONFIG_BIAS_PULL_PIN_DEFAULT:
    case PIN_CONFIG_BIAS_PULL_DOWN:
    case PIN_CONFIG_BIAS_DISABLE:
      return pca953x_gpio_set_pull_up_down(chip, offset, config);
    default:
      return -ENOTSUPP;
  }
}

static void pca953x_setup_gpio(struct pca953x_chip *chip, int gpios) {
  struct gpio_chip *gc;

  gc = &chip->gpio_chip;

  gc->direction_input = pca953x_gpio_direction_input;
  gc->direction_output = pca953x_gpio_direction_output;
  gc->get = pca953x_gpio_get_value;
  gc->set = pca953x_gpio_set_value;
  gc->get_direction = pca953x_gpio_get_direction;
  gc->get_multiple = pca953x_gpio_get_multiple;
  gc->set_multiple = pca953x_gpio_set_multiple;
  gc->set_config = pca953x_gpio_set_config;
  gc->can_sleep = true;

  gc->base = chip->gpio_start;
  gc->ngpio = gpios;
  gc->label = dev_name(&chip->client->dev);
  gc->parent = &chip->client->dev;
  gc->owner = THIS_MODULE;
  gc->names = chip->names;
}

static void pca953x_irq_mask(struct irq_data *d) {
  struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
  struct pca953x_chip *chip = gpiochip_get_data(gc);
  irq_hw_number_t hwirq = irqd_to_hwirq(d);

  clear_bit(hwirq, chip->irq_mask);
}

static void pca953x_irq_unmask(struct irq_data *d) {
  struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
  struct pca953x_chip *chip = gpiochip_get_data(gc);
  irq_hw_number_t hwirq = irqd_to_hwirq(d);

  set_bit(hwirq, chip->irq_mask);
}

static int pca953x_irq_set_wake(struct irq_data *d, unsigned int on) {
  struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
  struct pca953x_chip *chip = gpiochip_get_data(gc);

  if (on)
    atomic_inc(&chip->wakeup_path);
  else
    atomic_dec(&chip->wakeup_path);

  return irq_set_irq_wake(chip->client->irq, on);
}

static void pca953x_irq_bus_lock(struct irq_data *d) {
  struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
  struct pca953x_chip *chip = gpiochip_get_data(gc);

  mutex_lock(&chip->irq_lock);
}

static void pca953x_irq_bus_sync_unlock(struct irq_data *d) {
  struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
  struct pca953x_chip *chip = gpiochip_get_data(gc);
  DECLARE_BITMAP(irq_mask, MAX_LINE);
  DECLARE_BITMAP(reg_direction, MAX_LINE);
  int level;

  if (chip->driver_data & PCA_PCAL) {
    /* Enable latch on interrupt-enabled inputs */
    pca953x_write_regs(chip, PCAL953X_IN_LATCH, chip->irq_mask);

    bitmap_complement(irq_mask, chip->irq_mask, gc->ngpio);

    /* Unmask enabled interrupts */
    pca953x_write_regs(chip, PCAL953X_INT_MASK, irq_mask);
  }

  /* Switch direction to input if needed */
  pca953x_read_regs(chip, chip->regs->direction, reg_direction);

  bitmap_or(irq_mask, chip->irq_trig_fall, chip->irq_trig_raise, gc->ngpio);
  bitmap_complement(reg_direction, reg_direction, gc->ngpio);
  bitmap_and(irq_mask, irq_mask, reg_direction, gc->ngpio);

  /* Look for any newly setup interrupt */
  for_each_set_bit(level, irq_mask, gc->ngpio)
  pca953x_gpio_direction_input(&chip->gpio_chip, level);

  mutex_unlock(&chip->irq_lock);
}

static int pca953x_irq_set_type(struct irq_data *d, unsigned int type) {
  struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
  struct pca953x_chip *chip = gpiochip_get_data(gc);
  irq_hw_number_t hwirq = irqd_to_hwirq(d);

  if (!(type & IRQ_TYPE_EDGE_BOTH)) {
    dev_err(&chip->client->dev, "irq %d: unsupported type %d\n",
            d->irq, type);
    return -EINVAL;
  }

  assign_bit(hwirq, chip->irq_trig_fall, type & IRQ_TYPE_EDGE_FALLING);
  assign_bit(hwirq, chip->irq_trig_raise, type & IRQ_TYPE_EDGE_RISING);

  return 0;
}

static void pca953x_irq_shutdown(struct irq_data *d) {
  struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
  struct pca953x_chip *chip = gpiochip_get_data(gc);
  irq_hw_number_t hwirq = irqd_to_hwirq(d);

  clear_bit(hwirq, chip->irq_trig_raise);
  clear_bit(hwirq, chip->irq_trig_fall);
}

static bool pca953x_irq_pending(struct pca953x_chip *chip, unsigned long *pending) {
  struct gpio_chip *gc = &chip->gpio_chip;
  DECLARE_BITMAP(reg_direction, MAX_LINE);
  DECLARE_BITMAP(old_stat, MAX_LINE);
  DECLARE_BITMAP(cur_stat, MAX_LINE);
  DECLARE_BITMAP(new_stat, MAX_LINE);
  DECLARE_BITMAP(trigger, MAX_LINE);
  int ret;

  if (chip->driver_data & PCA_PCAL) {
    /* Read the current interrupt status from the device */
    ret = pca953x_read_regs(chip, PCAL953X_INT_STAT, trigger);
    if (ret)
      return false;

    /* Check latched inputs and clear interrupt status */
    ret = pca953x_read_regs(chip, chip->regs->input, cur_stat);
    if (ret)
      return false;

    /* Apply filter for rising/falling edge selection */
    bitmap_replace(new_stat, chip->irq_trig_fall, chip->irq_trig_raise, cur_stat, gc->ngpio);

    bitmap_and(pending, new_stat, trigger, gc->ngpio);

    return !bitmap_empty(pending, gc->ngpio);
  }

  ret = pca953x_read_regs(chip, chip->regs->input, cur_stat);
  if (ret)
    return false;

  /* Remove output pins from the equation */
  pca953x_read_regs(chip, chip->regs->direction, reg_direction);

  bitmap_copy(old_stat, chip->irq_stat, gc->ngpio);

  bitmap_and(new_stat, cur_stat, reg_direction, gc->ngpio);
  bitmap_xor(cur_stat, new_stat, old_stat, gc->ngpio);
  bitmap_and(trigger, cur_stat, chip->irq_mask, gc->ngpio);

  bitmap_copy(chip->irq_stat, new_stat, gc->ngpio);

  if (bitmap_empty(trigger, gc->ngpio))
    return false;

  bitmap_and(cur_stat, chip->irq_trig_fall, old_stat, gc->ngpio);
  bitmap_and(old_stat, chip->irq_trig_raise, new_stat, gc->ngpio);
  bitmap_or(new_stat, old_stat, cur_stat, gc->ngpio);
  bitmap_and(pending, new_stat, trigger, gc->ngpio);

  return !bitmap_empty(pending, gc->ngpio);
}

static int process_inputs(struct gpio_chip *gpio_chip);

static struct config {
  struct gpio_chip *gpio_chip_0;
  struct gpio_chip *gpio_chip_1;
  struct gpio_chip *gpio_chip_2;
  struct task_struct *config_thread;
  struct task_struct *debounce_thread;
  struct mutex process_mutex;
  bool hotkey_patterns;
  bool disable_credit_on_hk_btn1;
  bool reverse_credit_logic;
  uint buttons_on_jamma;
  bool btn_6_auto_disabled;
} jamma_config;

static irqreturn_t pca953x_irq_handler(int irq, void *devid) {

  struct pca953x_chip *chip = (struct pca953x_chip *)devid;
  if(&chip->gpio_chip == jamma_config.gpio_chip_0){
    DEBUG && printk(KERN_INFO
    "recalboxrgbjamma: IRQ triggered on chip 0\n");
    process_inputs(jamma_config.gpio_chip_0);
  } else if(&chip->gpio_chip == jamma_config.gpio_chip_1){
    DEBUG && printk(KERN_INFO
    "recalboxrgbjamma: IRQ triggered on chip 1\n");
    process_inputs(jamma_config.gpio_chip_1);
  } else {
    DEBUG && printk(KERN_INFO
    "recalboxrgbjamma: IRQ triggered on chip 2\n");
    process_inputs(jamma_config.gpio_chip_2);
  }

  return IRQ_RETVAL(1);
}

static int pca953x_irq_setup(struct pca953x_chip *chip, int irq_base) {
  struct i2c_client *client = chip->client;
  struct irq_chip *irq_chip = &chip->irq_chip;
  DECLARE_BITMAP(reg_direction, MAX_LINE);
  DECLARE_BITMAP(irq_stat, MAX_LINE);
  struct gpio_irq_chip *girq;
  int ret;

  printk(KERN_INFO
  "recalboxrgbjamma: setting IRQ\n");

  if (!client->irq)
    return 0;

  if (irq_base == -1)
    return 0;

  if (!(chip->driver_data & PCA_INT))
    return 0;

  ret = pca953x_read_regs(chip, chip->regs->input, irq_stat);
  if (ret)
    return ret;

  /*
   * There is no way to know which GPIO line generated the
   * interrupt.  We have to rely on the previous read for
   * this purpose.
   */
  pca953x_read_regs(chip, chip->regs->direction, reg_direction);
  bitmap_and(chip->irq_stat, irq_stat, reg_direction, chip->gpio_chip.ngpio);
  mutex_init(&chip->irq_lock);

  irq_chip->name = dev_name(&client->dev);
  irq_chip->irq_mask = pca953x_irq_mask;
  irq_chip->irq_unmask = pca953x_irq_unmask;
  irq_chip->irq_set_wake = pca953x_irq_set_wake;
  irq_chip->irq_bus_lock = pca953x_irq_bus_lock;
  irq_chip->irq_bus_sync_unlock = pca953x_irq_bus_sync_unlock;
  irq_chip->irq_set_type = pca953x_irq_set_type;
  irq_chip->irq_shutdown = pca953x_irq_shutdown;

  girq = &chip->gpio_chip.irq;
  girq->chip = irq_chip;
  /* This will let us handle the parent IRQ in the driver */
  girq->parent_handler = NULL;
  girq->num_parents = 0;
  girq->parents = NULL;
  girq->default_type = IRQ_TYPE_NONE;
  girq->handler = handle_simple_irq;
  girq->threaded = true;
  girq->first = irq_base; /* FIXME: get rid of this */

  ret = devm_request_threaded_irq(&client->dev, client->irq,
                                  NULL, pca953x_irq_handler,
                                  IRQF_ONESHOT | IRQF_SHARED,
                                  dev_name(&client->dev), chip);
  if (ret) {
    dev_err(&client->dev, "failed to request irq %d\n",
            client->irq);
    return ret;
  }

  return 0;
}

static int device_pca95xx_init(struct pca953x_chip *chip, u32 invert) {
  DECLARE_BITMAP(val, MAX_LINE);
  u8 regaddr;
  int ret;

  regaddr = pca953x_recalc_addr(chip, chip->regs->output, 0);
  ret = regcache_sync_region(chip->regmap, regaddr,
                             regaddr + NBANK(chip) - 1);
  if (ret)
    goto out;

  regaddr = pca953x_recalc_addr(chip, chip->regs->direction, 0);
  ret = regcache_sync_region(chip->regmap, regaddr,
                             regaddr + NBANK(chip) - 1);
  if (ret)
    goto out;

  /* set platform specific polarity inversion */
  if (invert)
    bitmap_fill(val, MAX_LINE);
  else
    bitmap_zero(val, MAX_LINE);

  ret = pca953x_write_regs(chip, chip->regs->invert, val);
  out:
  return ret;
}


/*
 * A,B,X,Y are in SNES notation
 *
 * Chip 0 : address = 0x25
 *                                    __________
 *   P1K-B4 (P1-X)       IO0-0  <--- |0        8| ---> IO1-0  P1-B4    (P1-X)
 *   P1K-B5 (P1-L)       IO0-1  <--- |1        9| ---> IO1-1  P2-B4    (P2-X)
 *   P1K-B6 (P1-R)       IO0-2  <--- |2       10| ---> IO1-2  P1-B5    (P1-L)
 *   P2-B6  (P2-R)       IO0-3  <--- |3       11| ---> IO1-3  P2-B5    (P2-L)
 *   P1-B6  (P1-R)       IO0-4  <--- |4       12| ---> IO1-4  P3K-B3   (P3-Y)
 *   P2K-B4 (P2-X)       IO0-5  <--- |5       13| ---> IO1-5  P3K-LOCK (P3-LOCK)
 *   P2K-B5 (P2-L)       IO0-6  <--- |6       14| ---> IO1-6  P3K-START
 *   P2K-B5 (P2-R)       IO0-7  <--- |7       15| ---> IO1-7  P3K-UP
 *                                    ‾‾‾‾‾‾‾‾‾‾
 * Chip 1 : address = 0x24
 *                                    __________
 *   P4K-RIGHT           IO0-0  <--- |16      24| ---> IO1-0  P1-B3 (P1-Y)
 *   P4K-LEFT            IO0-1  <--- |17      25| ---> IO1-1  P2-B3 (P2-Y)
 *   P4K-DOWN            IO0-2  <--- |18      26| ---> IO1-2  P1-B2
 *   P4K-B1              IO0-3  <--- |19      27| ---> IO1-3  P2-B2
 *   P4K-B3              IO0-4  <--- |20      28| ---> IO1-4  P4K-LOCK
 *   P4K-B4              IO0-5  <--- |21      29| ---> IO1-5  P4K-UP
 *   P4K-B2              IO0-6  <--- |22      30| ---> IO1-6  P4K-START
 *   P3K-B4              IO0-7  <--- |23      31| ---> IO1-7  P4K-COIN
 *                                    ‾‾‾‾‾‾‾‾‾‾
 *                                    ‾‾‾‾‾‾‾‾‾‾
 * Chip 3 : address = 0x28
 *                                    __________
 *   P2-B1               IO0-0  <--- |32      40| ---> IO1-0  P2-UP
 *   P1-B1               IO0-1  <--- |33      41| ---> IO1-1  P1-UP
 *   P1-DOWN             IO0-2  <--- |34      42| ---> IO1-2  P2-START
 *   P2-DOWN             IO0-3  <--- |35      43| ---> IO1-3  P1-START
 *   P1-LEFT             IO0-4  <--- |36      44| ---> IO1-4  P2-COIN
 *   P2-LEFT             IO0-5  <--- |37      45| ---> IO1-5  P1-COIN
 *   P1-RIGHT            IO0-6  <--- |38      46| ---> IO1-6  TEST
 *   P2-RIGHT            IO0-7  <--- |39      47| ---> IO1-7  SERVICE
 *                                    ‾‾‾‾‾‾‾‾‾‾
 *
 */

#define TOTAL_PLAYERS 2

struct input_dev *player_devs[TOTAL_PLAYERS];

// Standard buttons mapping
#define BTN_PER_PLAYER 11
#define BTN_PER_PLAYER_ON_JAMMA 10
#define TOTAL_GPIO_ON_PCA 48

// Keep in mind the vanilla mapping for Recalbox RGB Jamma is (SNES notation):
// B A Y
// X L R
// We use this mapping to get B and A on first line, whatever the number of buttons
// Keep also in mind that the A is SOUTH and B is EAST (they are switched) on Linux notation, and that NORTH and WEST are also reversed in auto mapping...
//
static const unsigned int buttons_codes[BTN_PER_PLAYER] = {
    //  B1        B2         B3         B4         B5          B6          START          COIN            SERVICE         TEST             HOTKEY
    BTN_A, BTN_B, BTN_X, BTN_Y, BTN_TL, BTN_TR, BTN_START, BTN_SELECT, BTN_THUMBL, BTN_THUMBR, BTN_MODE,
};
// Games buttons are from 0 to 5 in buttons codes
#define LAST_GAME_BUTTON 5


#define PLAYER1 0
#define PLAYER2 1
#define JAMMA_BTNS 0
#define KICK_BTNS 1

static const int buttons_bits[2][2][BTN_PER_PLAYER_ON_JAMMA] = {
  // Player 1
  {
    // The value is the bit of the data containing the
    // information for the button at the same index in buttons_codes array
    // Start is a special case
    {33, 26, 24, 8, 10, 4, -1, 45, 47, 46,},
    // Kick harness buttons mapping
    // Only use values where >= 0
    {-1, -1, -1, 0, 1, 2, -1, -1, -1, -1,},
  },
  // Player 2
  {
    {32, 27, 25, 9, 11, 3, 42, 44, -1, -1,},
    {-1, -1, -1, 5, 6, 7, -1, -1, -1, -1,},
  }
};

#define P1_START 43
#define P1_BTN1 buttons_bits[PLAYER1][JAMMA_BTNS][0]
#define P2_BTN1 buttons_bits[PLAYER2][JAMMA_BTNS][0]

#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

static const int direction_bits[2][4]= {
  // Player 1
    {41,34,36,38},
    {40,35,37,39},
};



// Logic is not the same for each buttons (coins and service/test are reversed)
// TODO: add service and test for next version
static const unsigned short buttonsReleasedValues[TOTAL_GPIO_ON_PCA] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0
};
// TODO: add service and test for next version
static const unsigned short buttonsReleasedValuesReversed[TOTAL_GPIO_ON_PCA] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 0, 0, 0, 0
};


#define PRESSED(data, btn) ((btn) != -1 ? (((data >> (btn)) & 1)^(jamma_config.reverse_credit_logic ? buttonsReleasedValuesReversed[btn]: buttonsReleasedValues[btn])) : 0)
#define WAIT_SYNC() usleep_range(50000,80000);
//Exit delay (HK + START) set to 2 secondes
#define EXIT_DELAY(now, start) ((now - start) / 1000000000UL >= 2)
//HK delay set to 1 second
#define HOTKEY_DELAY(now, start) ((now - start) / 1000000000UL >= 1)
static long long int last_start_press = 0;
static unsigned long start_state = 0;
static unsigned int start_credit = 0;
static short can_exit = 0;
static short should_release_hk = 0;

#define PRESS_AND_SYNC(player, button) input_report_key(player_devs[(player)], (button), 1); input_sync(player_devs[(player)]); WAIT_SYNC();
#define RELEASE_AND_SYNC(player, button) input_report_key(player_devs[(player)], (button), 0); input_sync(player_devs[(player)]); WAIT_SYNC();


/**
 *
 * @param data_chips the data read from both chips
 * The right 16 bits are for the chip 1, left 16 bits for the chip 0
 */
static void input_report(unsigned long long *data_chips, long long int *time_ns) {
  int player, buttonIndex, buttonValue;

  if (jamma_config.hotkey_patterns) {
    // Special case for HOTKEY:
    // Simple press + release of START -> START event
    // Simple press of START + BTN1 -> send SELECT (COIN)
    // Simple press of START + an other button -> send HOTKEY + BUTTON
    // Long press (3s) of START -> HOTKEY + START

    if (PRESSED(*data_chips, P1_START)) {
      DEBUG && printk(KERN_INFO
      "recalboxrgbjamma: start is pressed\n");
      if (last_start_press == 0) {
        // First press
        DEBUG && printk(KERN_INFO
        "recalboxrgbjamma: saving first press\n");
        last_start_press = *time_ns;
        start_state = *data_chips;
        can_exit = 1;
        start_credit = 0;
      } else {
        DEBUG && printk(KERN_INFO
        "recalboxrgbjamma: startstate=%u data_chips=%u different=%d B1=%d\n", start_state, *data_chips, start_state !=
                                                                                                       *data_chips,
            PRESSED(
                *data_chips,
                P1_BTN1));
        if (start_state != *data_chips /*&& !start_credit*/) {
          // If we can use START + BTN1 for a credit
          if (!jamma_config.disable_credit_on_hk_btn1 && PRESSED(*data_chips, P1_BTN1)) {
            DEBUG && printk(KERN_INFO
            "recalboxrgbjamma: credit (SELECT) triggered (hk+P1_BTN1) B1=%u\n", PRESSED(*data_chips, P1_BTN1));
            PRESS_AND_SYNC(PLAYER1, BTN_SELECT);
            RELEASE_AND_SYNC(PLAYER1, BTN_SELECT);
            start_credit = 1;
            can_exit = 0;
          } if (!jamma_config.disable_credit_on_hk_btn1 && PRESSED(*data_chips, P2_BTN1)) {
            DEBUG && printk(KERN_INFO
            "recalboxrgbjamma: credit (SELECT) triggered (hk+P2_BTN1) B1=%u\n", PRESSED(*data_chips, P1_BTN1));
            PRESS_AND_SYNC(PLAYER2, BTN_SELECT);
            RELEASE_AND_SYNC(PLAYER2, BTN_SELECT);
            start_credit = 1;
            can_exit = 0;
          } else if (!start_credit && !should_release_hk) {
            // As another button has been pressed, we press hotkey before
            DEBUG && printk(KERN_INFO
            "recalboxrgbjamma: other button press (hk+x)\n");
            PRESS_AND_SYNC(PLAYER1, BTN_MODE);
            can_exit = 0;
            should_release_hk = 1;
          }
        }
        start_state = *data_chips;

      }
    } else {
      if (last_start_press != 0) {
        // Start released
        if (!start_credit) {
          DEBUG && printk(KERN_INFO
          "recalboxrgbjamma: start released\n");
          if (should_release_hk) {
            DEBUG && printk(KERN_INFO
            "recalboxrgbjamma: release hotkey\n");
            should_release_hk = 0;
            RELEASE_AND_SYNC(PLAYER1, BTN_MODE);
          } else if (can_exit && EXIT_DELAY(*time_ns, last_start_press)) {
            // Longest press, so we send both HK + START
            DEBUG && printk(KERN_INFO
            "recalboxrgbjamma: long press : sending HK + START\n");
            PRESS_AND_SYNC(PLAYER1, BTN_MODE);
            PRESS_AND_SYNC(PLAYER1, BTN_START);
            RELEASE_AND_SYNC(PLAYER1, BTN_START);
            RELEASE_AND_SYNC(PLAYER1, BTN_MODE);
          } else if (HOTKEY_DELAY(*time_ns, last_start_press)) {
            // Long press, so we send HK
            DEBUG && printk(KERN_INFO
            "recalboxrgbjamma: middle press : sending HK\n");
            PRESS_AND_SYNC(PLAYER1, BTN_MODE);
            RELEASE_AND_SYNC(PLAYER1, BTN_MODE);
          } else {
            // Simple start
            DEBUG && printk(KERN_INFO
            "recalboxrgbjamma: quick press : sending START\n");
            PRESS_AND_SYNC(PLAYER1, BTN_START);
            RELEASE_AND_SYNC(PLAYER1, BTN_START);
          }
        } else {
          // Start have been released after a credit
          start_credit = 0;
        }
        last_start_press = 0;
      }
    }
  } else {
    // Standard start
    input_report_key(player_devs[PLAYER1], BTN_START, PRESSED(*data_chips, P1_START));
  }

  for(player = 0; player < TOTAL_PLAYERS; player++){
    if (player > 0 || !start_credit) {
      input_report_abs(player_devs[player], ABS_Y, PRESSED(*data_chips, direction_bits[player][DIR_DOWN]) - PRESSED(*data_chips, direction_bits[player][DIR_UP]));
      input_report_abs(player_devs[player], ABS_X, PRESSED(*data_chips, direction_bits[player][DIR_RIGHT]) - PRESSED(*data_chips, direction_bits[player][DIR_LEFT]));

      for (buttonIndex = 0; buttonIndex < BTN_PER_PLAYER_ON_JAMMA; buttonIndex++) {
        // If we are on a game button that is not on jamma (because user config)
        // or if the btn6 has been auto disabled (at startup because on ground)
        // then we use kickharness
        if ((buttonIndex <= LAST_GAME_BUTTON && buttonIndex >= jamma_config.buttons_on_jamma) ||
            (buttonIndex == 5 && jamma_config.btn_6_auto_disabled)) {
          buttonValue = PRESSED(*data_chips, buttons_bits[player][KICK_BTNS][buttonIndex]);
          /* DEBUG && printk(KERN_INFO "recalboxrgbjamma: sending report P1 for key bit:%d, inputcode:%d, value : %d\n", player1_kick_btn_bits[j],
             buttons_codes[j], PRESSED(data_chips, player1_kick_btn_bits[j]));*/
        } else {
          // let's read on both inputs
          buttonValue = PRESSED(*data_chips, buttons_bits[player][JAMMA_BTNS][buttonIndex]) | PRESSED(*data_chips, buttons_bits[player][KICK_BTNS][buttonIndex]);
        }
        input_report_key(player_devs[player], buttons_codes[buttonIndex], buttonValue);
      }
      input_sync(player_devs[player]);
    }
  }
}

static unsigned long lastGpioState = 0;
static unsigned long debounceGpioValue = 0;
static long long int lastActionOn[TOTAL_GPIO_ON_PCA] = {0};

#define SETGPIO(data_addr, gpio, value) (*(data_addr) = ((*(data_addr) & (~(1UL << (gpio)))) | ((value) << (gpio))))

static int debounce(unsigned long long *data_chips, unsigned long long *debounced_data, long long int *time_ns) {
  unsigned long gpioVal;
  *debounced_data = *data_chips;
  unsigned int gpio;
  for (gpio = 0; gpio < TOTAL_GPIO_ON_PCA; gpio++) {
    if (lastActionOn[gpio] != 0) {
      // We are in debounce time lap
      // We cancel this event by setting the saved value at debounce start
      DEBUG && printk(KERN_INFO
      "recalboxrgbjamma: debouncing: setting gpio %d value to %d (%s) in place of %d\n", gpio,
          (debounceGpioValue >> gpio) &
          1, ((debounceGpioValue >> gpio) & 1) ? "released" : "pressed", gpioVal);
      SETGPIO(debounced_data, gpio, (debounceGpioValue >> gpio) & 1);
    } else {
      gpioVal = (((*data_chips) >> gpio) & 1);
      if (((lastGpioState >> gpio) & 1) != gpioVal) {
        // Gpio changed its state since last event
        // We set the start time for debounce and the value of the gpio
        lastActionOn[gpio] = *time_ns;
        // Saving the value that will be still during the debounce
        DEBUG && printk(KERN_INFO
        "recalboxrgbjamma: debouncing: first event of gpio %d, setting debounced value to %d \n", gpio, gpioVal);
        SETGPIO(&debounceGpioValue, gpio, gpioVal);
      }
    }
  }
  lastGpioState = *data_chips;
  return 0;
}
static const unsigned long mask = 0xFFFF;

static int read_gpios(unsigned long long *gpio_data) {
  unsigned long gpio_values0 = 0;
  unsigned long gpio_values1 = 0;
  unsigned long gpio_values2 = 0;
  if (jamma_config.gpio_chip_0 != NULL) {
    if (pca953x_gpio_get_multiple(jamma_config.gpio_chip_0, &mask, &gpio_values0)) {
      printk(KERN_INFO
      "recalboxrgbjamma: unable to read gpio on chip 0, skipping\n");
      return -1;
    };
  }
  if (jamma_config.gpio_chip_1 != NULL) {
    if (pca953x_gpio_get_multiple(jamma_config.gpio_chip_1, &mask, &gpio_values1)) {
      printk(KERN_INFO
      "recalboxrgbjamma: unable to read gpio chip 1, skipping\n");
      return -1;
    }
  }
  if (jamma_config.gpio_chip_2 != NULL) {
    if (pca953x_gpio_get_multiple(jamma_config.gpio_chip_2, &mask, &gpio_values2)) {
      printk(KERN_INFO
      "recalboxrgbjamma: unable to read gpio chip 2, skipping\n");
      return -1;
    }
  }
  *gpio_data = (gpio_values2 << 32) | (gpio_values1 << 16) | gpio_values0;
  return 0;
}

static int read_gpios_one_chip(unsigned long *gpio_data, struct gpio_chip *gpio_chip) {
  if (pca953x_gpio_get_multiple(gpio_chip, &mask, gpio_data)) {
    printk(KERN_INFO
    "recalboxrgbjamma: unable to read gpio on chip %s, skipping\n", gpio_chip == jamma_config.gpio_chip_0 ? "0":(gpio_chip == jamma_config.gpio_chip_1 ? "1" : "2"));
    return -1;
  };
  return 0;
}


// Debounce to 40ms
#define DEBOUNCE_TIME (40000000)

static unsigned long long gpio_data = 0;

static int process_inputs(struct gpio_chip *gpio_chip) {
  unsigned long long debounced_data = 0;
  unsigned long gpio_data_chip = 0;
  long long int time_start = ktime_to_ns(ktime_get_boottime());
  mutex_lock(&jamma_config.process_mutex);

  if(gpio_chip != NULL) {
    if (read_gpios_one_chip(&gpio_data_chip, gpio_chip)) {
      mutex_unlock(&jamma_config.process_mutex);
      return -1;
    }
    DEBUG && printk(KERN_INFO
    "recalboxrgbjamma: read gpio values on chip : %u\n", gpio_data_chip);
    if(gpio_chip == jamma_config.gpio_chip_0){
      gpio_data = ((gpio_data & 0xFFFFFFFF0000) | gpio_data_chip);
    } else if(gpio_chip == jamma_config.gpio_chip_1) {
      gpio_data = ((gpio_data & 0xFFFF0000FFFF) | (gpio_data_chip << 16));
    } else if(gpio_chip == jamma_config.gpio_chip_2) {
      gpio_data = ((gpio_data & 0x0000FFFFFFFF) | (gpio_data_chip << 32));
    }

    DEBUG && printk(KERN_INFO
    "recalboxrgbjamma: gpio_data = %u\n", gpio_data);
  } else {
    if (read_gpios(&gpio_data)) {
      mutex_unlock(&jamma_config.process_mutex);
      return -1;
    }
  }

  debounce(&gpio_data, &debounced_data, &time_start);
  input_report(&debounced_data, &time_start);
  mutex_unlock(&jamma_config.process_mutex);
  return 0;
}

static int process_debounce(void *idx) {
  unsigned int gpio;
  long long int time_ns;
  bool sendEvent;

  while (!kthread_should_stop()) {
    time_ns = ktime_to_ns(ktime_get_boottime());
    sendEvent = false;
    for (gpio = 0; gpio < TOTAL_GPIO_ON_PCA; gpio++) {
      if (lastActionOn[gpio] != 0) {
        if (time_ns - lastActionOn[gpio] > DEBOUNCE_TIME) {
          // This gpio must be debounced
          DEBUG && printk(KERN_INFO
          "recalboxrgbjamma: debouncing thread: finish debouncing of %d\n", gpio);
          sendEvent = 1;
          lastActionOn[gpio] = 0;
        }
      }
    }
    if (sendEvent) {
      // Maybe we should not debounce here
      process_inputs(NULL);
    }
    usleep_range(10000, 15000);
  }
  return 0;
}

static int unregister_controllers(void) {
  int player;
  for (player = 0; player < TOTAL_PLAYERS; player++) {
    if (player_devs[player] != NULL)
      input_unregister_device(player_devs[player]);
  }
  return 0;
}

static const char *controllers_names[2] = {
    "JammaControllerP1", "JammaControllerP2"
};

static int register_controllers(void) {
  int err = 0, i = 0, player = 0;
  for (player = 0; player < TOTAL_PLAYERS; player++) {
    if (!(player_devs[player] = input_allocate_device())) {
      pr_err("recalboxrgbjamma: not enough memory for input device\n");
      return -ENOMEM;
    }

    player_devs[player]->name = controllers_names[player];
    player_devs[player]->phys = controllers_names[player];
    player_devs[player]->id.bustype = BUS_I2C;
    player_devs[player]->id.vendor = 0x7262;
    player_devs[player]->id.product = 0x0A01;
    player_devs[player]->id.version = 0x0100;

    player_devs[player]->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);

    for (i = 0; i < 2; i++)
      input_set_abs_params(player_devs[player], ABS_X + i, -1, 1, 0, 0);
    for (i = 0; i < BTN_PER_PLAYER; i++)
      __set_bit(buttons_codes[i], player_devs[player]->keybit);

    if ((err = input_register_device(player_devs[player]))) {
      pr_err("recalboxrgbjamma: Cannot register input device\n");
      input_free_device(player_devs[player]);
      player_devs[player] = NULL;
      return -1;
    }
  }
  return 0;
}
// Load configuration from recalbox-crt-options.cfg
// Used to get Hotkey mode
#define READ_SIZE_MAX 2048
#define LINE_SIZE_MAX 256
static char read_buf[READ_SIZE_MAX];
static const char *config_path = "/boot/crt/recalbox-crt-options.cfg";

static int load_config(void) {
  struct file *fp = NULL;
  ssize_t read_size = 0;
  size_t cursor = 0;
  char line[LINE_SIZE_MAX];
  char optionname[LINE_SIZE_MAX];
  int optionvalue = 0;
  size_t line_start = 0;
  size_t line_len = 0;
  int scanret = 0;

  fp = filp_open(config_path, O_RDONLY, 0);
  if (IS_ERR(fp) || !fp) {
    //printk(KERN_INFO "recalboxrgbjamma: config file not found, skipping configuration loading\n");
    return 0;
  }

  read_size = kernel_read(fp, &read_buf, READ_SIZE_MAX, &fp->f_pos);
  if (read_size <= 0) {
    filp_close(fp, NULL);
    //printk(KERN_INFO "recalboxrgbjamma: empty config file found, skipping configuration loading\n");
    return 0;
  }
  filp_close(fp, NULL);
  for (cursor = 0; cursor < read_size; cursor++) {
    line[cursor - line_start] = read_buf[cursor];
    line_len++;
    if (line_len >= LINE_SIZE_MAX || read_buf[cursor] == '\n' || read_buf[cursor] == '\0') {
      if (line_len > 1 && line[0] != '#') {
        line[line_len - 1] = '\0';
        scanret = sscanf(line, "%s = %d", &optionname, &optionvalue);
        if (scanret == 2) {
          if (strcmp(optionname, "options.jamma.hk_patterns") == 0) {
            if (jamma_config.hotkey_patterns != optionvalue) {
              printk(KERN_INFO
              "recalboxrgbjamma: switch to %s mode\n",
                  optionvalue ? "hotkey_patterns" : "enable_hk_on_start");
              jamma_config.hotkey_patterns = optionvalue;
            }
          } else if (strcmp(optionname, "options.jamma.disable_credit_on_hk_btn1") == 0) {
            if (jamma_config.disable_credit_on_hk_btn1 != optionvalue) {
              printk(KERN_INFO
              "recalboxrgbjamma: switch to %s mode\n",
                  optionvalue ? "disable_credit_on_hk_btn1" : "enable_credit_on_hk_btn1");
              jamma_config.disable_credit_on_hk_btn1 = optionvalue;
            }
          } else if (strcmp(optionname, "options.jamma.buttons_on_jamma") == 0) {
            if (jamma_config.buttons_on_jamma != optionvalue) {
              printk(KERN_INFO
              "recalboxrgbjamma: switch buttons_on_jamma to %d\n",
                  optionvalue);
              jamma_config.buttons_on_jamma = optionvalue;
            }
          } else if (strcmp(optionname, "options.jamma.reverse_credit_logic") == 0) {
            if (jamma_config.reverse_credit_logic != optionvalue) {
              printk(KERN_INFO
              "recalboxrgbjamma: switch reverse_credit_logic to %d\n",
                  optionvalue);
              jamma_config.reverse_credit_logic = optionvalue;
            }
          }
        }
      }
      line_start += line_len;
      line_len = 0;
      memset(line, 0, 128);
    }
  }
  return 0;
}

static int watch_configuration(void *idx) {
  int gpio_value = 0;
  while (!kthread_should_stop()) {
    // Read file
    load_config();
    // Change switch 3/6 GPIO
    /*if (jamma_config.switch36_gpio_ref != NULL) {
      gpio_value = gpiod_get_value(jamma_config.switch36_gpio_ref);
      if (gpio_value != jamma_config.switch36_gpio_state) {
        jamma_config.switch36_gpio_state = gpio_value;
        printk(KERN_INFO
        "recalboxrgbjamma: switch to %s buttons mode\n", gpio_value ? "6" : "3");
      }
    }*/
    usleep_range(10000000, 15000000);
  }
  return 0;
}





static int pca953x_probe(struct i2c_client *client,
                         const struct i2c_device_id *i2c_id) {
  struct pca953x_platform_data *pdata;
  struct pca953x_chip *chip;
  int irq_base = 0;
  int ret;
  int gpio_idx = 0;
  u32 invert = 0;
  struct regulator *reg;
  const struct regmap_config *regmap_config;

  chip = devm_kzalloc(&client->dev, sizeof(*chip), GFP_KERNEL);
  if (chip == NULL)
    return -ENOMEM;

  pdata = dev_get_platdata(&client->dev);
  if (pdata) {
    irq_base = pdata->irq_base;
    chip->gpio_start = pdata->gpio_base;
    invert = pdata->invert;
    chip->names = pdata->names;
  } else {
    struct gpio_desc *reset_gpio;

    chip->gpio_start = -1;
    irq_base = 0;

    /*
     * See if we need to de-assert a reset pin.
     *
     * There is no known ACPI-enabled platforms that are
     * using "reset" GPIO. Otherwise any of those platform
     * must use _DSD method with corresponding property.
     */
    reset_gpio = devm_gpiod_get_optional(&client->dev, "reset",
                                         GPIOD_OUT_LOW);
    if (IS_ERR(reset_gpio))
      return PTR_ERR(reset_gpio);
  }

  chip->client = client;

  reg = devm_regulator_get(&client->dev, "vcc");
  if (IS_ERR(reg))
    return dev_err_probe(&client->dev, PTR_ERR(reg), "reg get err\n");

  ret = regulator_enable(reg);
  if (ret) {
    dev_err(&client->dev, "reg en err: %d\n", ret);
    return ret;
  }
  chip->regulator = reg;

  if (i2c_id) {
    chip->driver_data = i2c_id->driver_data;
  } else {
    const void *match;

    match = device_get_match_data(&client->dev);
    if (!match) {
      ret = -ENODEV;
      goto err_exit;
    }

    chip->driver_data = (uintptr_t) match;
  }

  i2c_set_clientdata(client, chip);

  pca953x_setup_gpio(chip, chip->driver_data & PCA_GPIO_MASK);

  regmap_config = &pca953x_i2c_regmap;

  chip->regmap = devm_regmap_init_i2c(client, regmap_config);
  if (IS_ERR(chip->regmap)) {
    ret = PTR_ERR(chip->regmap);
    goto err_exit;
  }

  regcache_mark_dirty(chip->regmap);

  mutex_init(&chip->i2c_lock);
  /*
   * In case we have an i2c-mux controlled by a GPIO provided by an
   * expander using the same driver higher on the device tree, read the
   * i2c adapter nesting depth and use the retrieved value as lockdep
   * subclass for chip->i2c_lock.
   *
   * REVISIT: This solution is not complete. It protects us from lockdep
   * false positives when the expander controlling the i2c-mux is on
   * a different level on the device tree, but not when it's on the same
   * level on a different branch (in which case the subclass number
   * would be the same).
   *
   * TODO: Once a correct solution is developed, a similar fix should be
   * applied to all other i2c-controlled GPIO expanders (and potentially
   * regmap-i2c).
   */
  lockdep_set_subclass(&chip->i2c_lock,
                       i2c_adapter_depth(client->adapter));

  /* initialize cached registers from their original values.
   * we can't share this chip with another i2c master.
   */

  chip->regs = &pca953x_regs;
  ret = device_pca95xx_init(chip, invert);
  if (ret)
    goto err_exit;

  ret = pca953x_irq_setup(chip, irq_base);
  if (ret)
    goto err_exit;

  ret = devm_gpiochip_add_data(&client->dev, &chip->gpio_chip, chip);
  if (ret)
    goto err_exit;

  if (pdata && pdata->setup) {
    ret = pdata->setup(client, chip->gpio_chip.base,
                       chip->gpio_chip.ngpio, pdata->context);
    if (ret < 0) {
      dev_warn(&client->dev, "setup failed, %d\n", ret);
      goto err_exit;
    }
  }
  int command_ret;
  for (gpio_idx = 0; gpio_idx < 16; gpio_idx++) {
    /*    command_ret = pca953x_gpio_set_config(&chip->gpio_chip, gpio_idx, PIN_CONFIG_BIAS_PULL_UP);
        if(command_ret != 0) {
          dev_err(&client->dev, "cannot set pull up for gpio %d on chip %d. error: %d\n", gpio_idx, client->addr, command_ret);
          goto err_exit;
        }*/
    if (pca953x_gpio_direction_input(&chip->gpio_chip, gpio_idx) != 0) {
      dev_err(&client->dev, "cannot set input for gpio %d on chip %d\n", gpio_idx, client->addr);
      goto err_exit;
    }
  }

  dev_info(&client->dev, "created jamma pca at %d\n", client->addr);
  if (client->addr == 0x21 || client->addr == 0x20 || client->addr == 0x25) {
    // Check if button 6 is on gnd, and if it is, we will ignore it
    jamma_config.gpio_chip_0 = &chip->gpio_chip;
    if (process_inputs(&chip->gpio_chip) == 0) {
      dev_info(&client->dev, "VALUE HERE : %llu\n", gpio_data);
      if(PRESSED(gpio_data, buttons_bits[PLAYER1][JAMMA_BTNS][5]) || PRESSED(gpio_data,buttons_bits[PLAYER2][JAMMA_BTNS][5])){
        dev_info(&client->dev, "disabled 6th button on jamma!\n");
        jamma_config.btn_6_auto_disabled = true;
      }
    }
  } else if (client->addr == 0x22 || client->addr == 0x24) {
    jamma_config.gpio_chip_1 = &chip->gpio_chip;
  } else if (client->addr == 0x27) {
    jamma_config.gpio_chip_2 = &chip->gpio_chip;
  }

  process_inputs(NULL);
  return 0;

  err_exit:
  regulator_disable(chip->regulator);
  return ret;
}

static int pca953x_remove(struct i2c_client *client) {
  struct pca953x_platform_data *pdata = dev_get_platdata(&client->dev);
  struct pca953x_chip *chip = i2c_get_clientdata(client);
  int ret;

  if (pdata && pdata->teardown) {
    ret = pdata->teardown(client, chip->gpio_chip.base,
                          chip->gpio_chip.ngpio, pdata->context);
    if (ret < 0)
      dev_err(&client->dev, "teardown failed, %d\n", ret);
  } else {
    ret = 0;
  }

  regulator_disable(chip->regulator);

  return ret;
}

static const struct of_device_id pca953x_dt_ids[] = {
  {.compatible = "raspberrypi,recalboxrgbjamma", .data = (void *) (16 | PCA953X_TYPE | PCA_INT),},
  {}
};

MODULE_DEVICE_TABLE(of, pca953x_dt_ids
);
static SIMPLE_DEV_PM_OPS(pca953x_pm_ops, pca953x_suspend, pca953x_resume
);

static struct i2c_driver pca953x_driver = {
  .driver = {
    .name  = "recalboxrgbjamma",
    .pm  = &pca953x_pm_ops,
    .of_match_table = pca953x_dt_ids,
    .acpi_match_table = pca953x_acpi_ids,
  },
  .probe     = pca953x_probe,
  .remove    = pca953x_remove,
  .id_table  = pca953x_id,
};

static int __init

pca953x_init(void) {
  int idx = 0;
  jamma_config.gpio_chip_0 = NULL;
  jamma_config.gpio_chip_1 = NULL;
  jamma_config.gpio_chip_2 = NULL;
  jamma_config.hotkey_patterns = true;
  jamma_config.disable_credit_on_hk_btn1 = false;
  jamma_config.reverse_credit_logic = false;
  jamma_config.buttons_on_jamma = 6;
  jamma_config.btn_6_auto_disabled = false;


  jamma_config.config_thread = kthread_create(watch_configuration, &idx, "kthread_recalboxrgbjamma_cfg");
  printk(KERN_INFO
  "recalboxrgbjamma: setting configuration thread\n");
  if (jamma_config.config_thread != NULL) {
    wake_up_process(jamma_config.config_thread);
    printk(KERN_INFO
    "recalboxrgbjamma: kthread_recalboxrgbjamma_cfg is running\n");
  } else {
    printk(KERN_ERR
    "recalboxrgbjamma: kthread kthread_recalboxrgbjamma_cfg could not be created\n");
    return -1;
  }

  jamma_config.debounce_thread = kthread_create(process_debounce, &idx, "kthread_recalboxrgbjamma_debounce");
  printk(KERN_INFO
  "recalboxrgbjamma: setting debounce thread\n");
  if (jamma_config.debounce_thread != NULL) {
    wake_up_process(jamma_config.debounce_thread);
    printk(KERN_INFO
    "recalboxrgbjamma: kthread_recalboxrgbjamma_debounce is running\n");
  } else {
    printk(KERN_ERR
    "recalboxrgbjamma: kthread_recalboxrgbjamma_debounce could not be created\n");
    return -1;
  }

  mutex_init(&jamma_config.process_mutex);
  printk(KERN_INFO
  "recalboxrgbjamma: registering controllers\n");
  register_controllers();
  return i2c_add_driver(&pca953x_driver);
}

/* register after i2c/ postcore initcall and before
 * subsys initcalls that may rely on these GPIOs
 */
subsys_initcall(pca953x_init);

static void __exit

pca953x_exit(void) {
  printk(KERN_INFO
  "recalboxrgbjamma: exiting\n");
  if (kthread_stop(jamma_config.config_thread)) {
    printk("recalboxrgbjamma: can't stop config thread");
  }
  if (kthread_stop(jamma_config.debounce_thread)) {
    printk("recalboxrgbjamma: can't stop debounce thread");
  }
  unregister_controllers();
  i2c_del_driver(&pca953x_driver);
}

module_exit(pca953x_exit);

MODULE_AUTHOR("eric miao <eric.miao@marvell.com>, digitalLumberjack <digitalLumberjack@recalbox.com>");
MODULE_DESCRIPTION("Recalbox RGB Jamma controllers driver");
MODULE_LICENSE("GPL");