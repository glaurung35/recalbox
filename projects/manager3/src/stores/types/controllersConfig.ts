/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const ControllersConfigSchema = z.object({
  'xarcade.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'bluetooth.ertm': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'gpio.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'ps3.driver': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'steam.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'gamecon.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'ps3.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'gpio.args': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'gamecon.args': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'bluetooth.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'db9.args': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'db9.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'joycond.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  swapvalidateandcancel: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
});

export type ControllersConfig = z.infer<typeof ControllersConfigSchema>

export const ControllersConfigOptionsSchema = z.object({
  'xarcade.enabled': z.object({
    type: z.string(),
  }),
  'bluetooth.ertm': z.object({
    type: z.string(),
  }),
  'gpio.enabled': z.object({
    type: z.string(),
  }),
  'ps3.driver': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'steam.enabled': z.object({
    type: z.string(),
  }),
  'gamecon.enabled': z.object({
    type: z.string(),
  }),
  'ps3.enabled': z.object({
    type: z.string(),
  }),
  'gpio.args': z.object({
    type: z.string(),
  }),
  'gamecon.args': z.object({
    type: z.string(),
  }),
  'bluetooth.enabled': z.object({
    type: z.string(),
  }),
  'db9.args': z.object({
    type: z.string(),
  }),
  'db9.enabled': z.object({
    type: z.string(),
  }),
  'joycond.enable': z.object({
    type: z.string(),
  }),
  swapvalidateandcancel: z.object({
    type: z.string(),
  }),
});

export type ControllersConfigOptions = z.infer<typeof ControllersConfigOptionsSchema>
