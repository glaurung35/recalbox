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

static const struct i2c_device_id pca953x_id[] = {
    {"recalboxrgbjamma", 16 | PCA953X_TYPE | PCA_INT,},
    {}
};
MODULE_DEVICE_TABLE(i2c, pca953x_id);

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
  for_each_set_bit(level, irq_mask, gc->ngpio)pca953x_gpio_direction_input(&chip->gpio_chip, level);

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

static int read_gpios(void);

static irqreturn_t pca953x_irq_handler(int irq, void *devid) {
  //printk(KERN_INFO "recalboxrgbjamma: IRQ declenched\n");
  read_gpios();
  return IRQ_RETVAL(1);
}

static int pca953x_irq_setup(struct pca953x_chip *chip, int irq_base) {
  struct i2c_client *client = chip->client;
  struct irq_chip *irq_chip = &chip->irq_chip;
  DECLARE_BITMAP(reg_direction, MAX_LINE);
  DECLARE_BITMAP(irq_stat, MAX_LINE);
  struct gpio_irq_chip *girq;
  int ret;

  printk(KERN_INFO "recalboxrgbjamma: setting IRQ\n");

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

static struct config {
  struct gpio_chip *gpio_chip_0;
  struct gpio_chip *gpio_chip_1;
  struct gpio_desc *switch36_gpio_ref;
  int switch36_gpio_state;
  struct task_struct *config_thread;
  bool disable_hk_on_start;
} jamma_config;

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
  if (client->addr == 0x20) {
    jamma_config.switch36_gpio_ref = gpiod_get((client->dev.parent), "switch36", GPIOD_IN);
    if (IS_ERR(jamma_config.switch36_gpio_ref)) {
      dev_err(&client->dev, "cannot get switch36 gpio reference, default to 6 buttons\n");
      jamma_config.switch36_gpio_ref = NULL;
      jamma_config.switch36_gpio_state = 1;
    } else {
      jamma_config.switch36_gpio_state = gpiod_get_value(jamma_config.switch36_gpio_ref);
      dev_err(&client->dev, "switch36 gpio reference found, set to = %s.\n",
              jamma_config.switch36_gpio_state ? "6 buttons" : "3 buttons");
    }
    jamma_config.gpio_chip_0 = &chip->gpio_chip;
  } else if (client->addr == 0x22) {
    jamma_config.gpio_chip_1 = &chip->gpio_chip;
  }

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

MODULE_DEVICE_TABLE(of, pca953x_dt_ids);
static SIMPLE_DEV_PM_OPS(pca953x_pm_ops, pca953x_suspend, pca953x_resume);

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

/*
 * Chip 0 : address = 0x20
 *                                    __________
 *   P1-B3  (P1-EAST)    IO0-0  <--- |0        8| ---> IO1-0  P1K-B5 (P1-L)
 *   P2-B3  (P2-EAST)    IO0-1  <--- |          | ---> IO1-1  P2K-B5 (P2-L)
 *   P2-B2  (P2-WEST)    IO0-2  <--- |          | ---> IO1-2  P1K-B4 (P1-NORTH)
 *   P1-B2  (P1-WEST)    IO0-3  <--- |          | ---> IO1-3  P2K-B4 (P2-NORTH)
 *   P2-B4  (P2-NORTH)   IO0-4  <--- |          | ---> IO1-4  P1-B6  (P1-R)
 *   P1-B4  (P1-NORTH)   IO0-5  <--- |          | ---> IO1-5  P2-B6  (P2-R)
 *   P1K-B6 (P1-R)       IO0-6  <--- |          | ---> IO1-6  P1-B5  (P1-L)
 *   P2K-B6 (P2-R)       IO0-7  <--- |7       15| ---> IO1-7  P2-B5  (P2-L)
 *                                    ‾‾‾‾‾‾‾‾‾‾
 * Chip 1 : address = 0x22
 *                                    __________
 *   P1-DOWN  (P1-DOWN)  IO0-0  <--- |16      24| ---> IO1-0  P2-COIN (P2-COIN)
 *   P2-DOWN  (P2-DOWN)  IO0-1  <--- |          | ---> IO1-1  P1-COIN (P1-COIN)
 *   P1-UP    (P1-UP)    IO0-2  <--- |          | ---> IO1-2  P2-LEFT (P2-LEFT)
 *   P2-UP    (P2-UP)    IO0-3  <--- |          | ---> IO1-3  P1-LEFT (P1-LEFT)
 *   P1-START (P1-START) IO0-4  <--- |          | ---> IO1-4  P2-RIGHT (P2-RIGHT)
 *   P2-START (P2-START) IO0-5  <--- |          | ---> IO1-5  P1-RIGHT (P1-RIGHT)
 *   SERVICE             IO0-6  <--- |          | ---> IO1-6  P2-B1 (P2-SOUTH)
 *   TEST                IO0-7  <--- |23      31| ---> IO1-7  P1-B1 (P1-SOUTH)
 *                                    ‾‾‾‾‾‾‾‾‾‾
 *
 */


struct input_dev *player_devs[2];

// Standard buttons mapping
#define BTN_PER_PLAYER 11
static const unsigned int buttons_codes[BTN_PER_PLAYER] = {
    BTN_B, BTN_A, BTN_Y, BTN_X, BTN_TL, BTN_TR, BTN_START, BTN_SELECT, BTN_EXTRA, BTN_MISC, BTN_MODE,
};

#define P1_UP 18
#define P1_DOWN 16
#define P1_LEFT 27
#define P1_RIGHT 29
#define P1_START 20

// The value is the bit of the data containing the
// information for the button at the same index in buttons_codes array
// Start is a special case
static const int player1_6btn_bits[BTN_PER_PLAYER] = {
    31, 0, 3, 5, 14, 12, -1, 25, 22, 23, -1,
};
// 3btn mode bits
static const int player1_3btn_bits[BTN_PER_PLAYER] = {
    31, 0, 3, -1, -1, -1, -1, 25, 22, 23, -1,
};
// Kick harness buttons mapping
// Only use values where >= 0
static const int player1_kick_btn_bits[BTN_PER_PLAYER] = {
    -1, -1, -1, 10, 8, 6, -1, -1, -1, -1, -1,
};

#define P2_UP 19
#define P2_DOWN 17
#define P2_LEFT 26
#define P2_RIGHT 28
static const int player2_6btn_bits[BTN_PER_PLAYER] = {
    30, 2, 1, 4, 15, 13, 21, 24, -1, -1, -1,
};
static const int player2_3btn_bits[BTN_PER_PLAYER] = {
    30, 2, 1, -1, -1, -1, 21, 24, -1, -1, -1,
};

static const int player2_kick_btn_bits[BTN_PER_PLAYER] = {
    -1, -1, -1, 11, 9, 7, -1, -1, -1, -1, -1,
};

#define PRESSED(data, btn) (!((data >> (btn)) & 1))
#define WAIT_SYNC() usleep_range(20000,30000);
//Exit delay (HK + START) set to 2 secondes
#define EXIT_DELAY(now, start) ((now - start) / 1000000000l >= 2)
//HK delay set to 1 second
#define HOTKEY_DELAY(now, start) ((now - start) / 1000000000l >= 1)
static long long int last_start_press = 0;
static unsigned int start_state = 0;
static short can_exit = 0;

/**
 *
 * @param data_chips the data read from both chips
 * The right 16 bits are for the chip 1, left 16 bits for the chip 0
 */
static void input_report(unsigned long data_chips, long long int *time_ns) {
  int j;
  int gpio;

  if (!jamma_config.disable_hk_on_start) {
    // Special case for HOTKEY:
    // Simple press + release of START -> START event
    // Simple press of START + an other button -> send HOTKEY + BUTTON
    // Long press (3s) of START -> HOTKEY + START

    if (PRESSED(data_chips, P1_START)) {
      if (last_start_press == 0) {
        // First press we should send Hotkey
        last_start_press = *time_ns;
        start_state = data_chips;
        can_exit = 1;
      } else {
        if (start_state != data_chips) {
          // As another button has been pressed, we press hotkey before
          input_report_key(player_devs[0], BTN_MODE, 1);
          input_sync(player_devs[0]);
          WAIT_SYNC();
          can_exit = 0;
        }
      }
    } else {
      if (last_start_press != 0) {
        // Start released
        if (can_exit && EXIT_DELAY(*time_ns, last_start_press)) {
          // Longest press, so we send both HK + START
          input_report_key(player_devs[0], BTN_MODE, 1);
          input_sync(player_devs[0]);
          WAIT_SYNC();
          input_report_key(player_devs[0], BTN_START, 1);
          input_sync(player_devs[0]);
          WAIT_SYNC();
          input_report_key(player_devs[0], BTN_START, 0);
          input_sync(player_devs[0]);
          WAIT_SYNC();
          input_report_key(player_devs[0], BTN_MODE, 0);
          input_sync(player_devs[0]);
          WAIT_SYNC();
        } else if (HOTKEY_DELAY(*time_ns, last_start_press)) {
          // Long press, so we send HK
          input_report_key(player_devs[0], BTN_MODE, 1);
          input_sync(player_devs[0]);
          WAIT_SYNC();
          input_report_key(player_devs[0], BTN_MODE, 0);
          input_sync(player_devs[0]);
          WAIT_SYNC();
        } else {
          // Simple start
          input_report_key(player_devs[0], BTN_START, 1);
          input_sync(player_devs[0]);
          WAIT_SYNC();
          input_report_key(player_devs[0], BTN_START, 0);
          input_sync(player_devs[0]);
          WAIT_SYNC();
        }
        last_start_press = 0;
      }
    }
  } else {
    // Standard start
    input_report_key(player_devs[0], BTN_START, PRESSED(data_chips, P1_START));
  }

  // Player 1
  input_report_abs(player_devs[0], ABS_Y, PRESSED(data_chips, P1_DOWN) - PRESSED(data_chips, P1_UP));
  input_report_abs(player_devs[0], ABS_X, PRESSED(data_chips, P1_RIGHT) - PRESSED(data_chips, P1_LEFT));

  for (j = 0; j < BTN_PER_PLAYER; j++) {
    //printk(KERN_INFO "recalboxrgbjamma: sending report P1 for key bit:%d, inputcode:%d, value : %d\n", player1_btn_bits[j],
    //       buttons_codes[j], PRESSED(data_chips, player1_btn_bits[j]));

    // Either 6btn mode code, or the 3btn, or the kick. No report if button is -1
    gpio = jamma_config.switch36_gpio_state ? player1_6btn_bits[j] : (player1_3btn_bits[j] != -1 ? player1_3btn_bits[j]
                                                                                                 : player1_kick_btn_bits[j]);
    if (gpio != -1) {
      input_report_key(player_devs[0], buttons_codes[j], PRESSED(data_chips, gpio));
    }
  }
  input_sync(player_devs[0]);

  // Player 2
  input_report_abs(player_devs[1], ABS_Y, PRESSED(data_chips, P2_DOWN) - PRESSED(data_chips, P2_UP));
  input_report_abs(player_devs[1], ABS_X, PRESSED(data_chips, P2_RIGHT) - PRESSED(data_chips, P2_LEFT));
  for (j = 0; j < BTN_PER_PLAYER; j++) {
    //printk(KERN_INFO "recalboxrgbjamma: sending report P2 for key bit:%d, inputcode:%d, value : %d\n", player2_btn_bits[j],
    //       buttons_codes[j], PRESSED(data_chips, player2_btn_bits[j]));
    // Either 6btn mode code, or the 3btn, or the kick. No report if button is -1
    gpio = jamma_config.switch36_gpio_state ? player2_6btn_bits[j] : (player2_3btn_bits[j] != -1 ? player2_3btn_bits[j]
                                                                                                 : player2_kick_btn_bits[j]);
    if (gpio != -1)
      input_report_key(player_devs[1], buttons_codes[j], PRESSED(data_chips, gpio));
  }
  input_sync(player_devs[1]);
}


static int read_gpios(void) {
  unsigned long gpio_values0 = 0;
  unsigned long gpio_values1 = 0;
  unsigned long mask = 0xFFFF;
  long long int time_start;
  long long int time_stop;
  time_start = ktime_to_ns(ktime_get_boottime());
  if (jamma_config.gpio_chip_0 != NULL) {
    pca953x_gpio_get_multiple(jamma_config.gpio_chip_0, &mask, &gpio_values0);
  }
  if (jamma_config.gpio_chip_1 != NULL) {
    pca953x_gpio_get_multiple(jamma_config.gpio_chip_1, &mask, &gpio_values1);
  }
  //time_stop = ktime_to_ns(ktime_get_boottime());
  //printk(KERN_INFO "recalboxrgbjamma: Time to read data on both chips : %lluµs\n", (time_stop - time_start) / 1000);

  input_report((gpio_values1 << 16) | gpio_values0, &time_start);
  //time_stop = ktime_to_ns(ktime_get_boottime());
  return 0;
}

static int unregister_controllers(void) {
  int player;
  for (player = 0; player < 2; player++) {
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
  for (player = 0; player < 2; player++) {
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
          if (strcmp(optionname, "jamma.disable_hk_on_start") == 0) {
            if (jamma_config.disable_hk_on_start != optionvalue) {
              printk(KERN_INFO "recalboxrgbjamma: switch to %s mode\n",
                     optionvalue ? "disable_hk_on_start" : "enable_hk_on_start");
              jamma_config.disable_hk_on_start = optionvalue;
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
    if (jamma_config.switch36_gpio_ref != NULL) {
      gpio_value = gpiod_get_value(jamma_config.switch36_gpio_ref);
      if (gpio_value != jamma_config.switch36_gpio_state) {
        jamma_config.switch36_gpio_state = gpio_value;
        printk(KERN_INFO "recalboxrgbjamma: switch to %s buttons mode\n", gpio_value ? "6" : "3");
      }
    }
    usleep_range(10000000, 15000000);
  }
  return 0;
}

static int __init pca953x_init(void) {
  const char *th_name = "kthread_recalboxrgbjamma";
  int idx = 0;
  jamma_config.switch36_gpio_ref = NULL;
  jamma_config.switch36_gpio_state = 1;
  jamma_config.gpio_chip_0 = NULL;
  jamma_config.gpio_chip_1 = NULL;
  jamma_config.disable_hk_on_start = false;
  jamma_config.config_thread = kthread_create(watch_configuration, &idx, th_name);

  printk(KERN_INFO "recalboxrgbjamma: setting configuration thread\n");
  if (jamma_config.config_thread != NULL) {
    wake_up_process(jamma_config.config_thread);
    printk(KERN_INFO "recalboxrgbjamma: %s is running\n", th_name);
  } else {
    printk(KERN_ERR "recalboxrgbjamma: kthread %s could not be created\n", th_name);
    return -1;
  }

  printk(KERN_INFO "recalboxrgbjamma: registering controllers\n");
  register_controllers();
  return i2c_add_driver(&pca953x_driver);
}

/* register after i2c/ postcore initcall and before
 * subsys initcalls that may rely on these GPIOs
 */
subsys_initcall(pca953x_init);

static void __exit pca953x_exit(void) {
  printk(KERN_INFO "recalboxrgbjamma: exiting\n");
  if (kthread_stop(jamma_config.config_thread)) {
    printk("recalboxrgbjamma: can't stop thread");
  }
  unregister_controllers();
  if (jamma_config.switch36_gpio_ref != NULL)
    gpiod_put(jamma_config.switch36_gpio_ref);
  i2c_del_driver(&pca953x_driver);
}

module_exit(pca953x_exit);

MODULE_AUTHOR("eric miao <eric.miao@marvell.com>, digitalLumberjack <digitalLumberjack@recalbox.com>");
MODULE_DESCRIPTION("Recalbox RGB Jamma controllers driver");
MODULE_LICENSE("GPL");