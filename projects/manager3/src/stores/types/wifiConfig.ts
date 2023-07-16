/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const WifiConfigSchema = z.object({
  enabled: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  netmask: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  key: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  ip: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  ssid: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  gateway: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  region: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  priority: z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  nameservers: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
});

export type WifiConfig = z.infer<typeof WifiConfigSchema>

export const WifiConfigOptionsSchema = z.object({
  enabled: z.object({
    type: z.string(),
  }),
  netmask: z.object({
    type: z.string(),
    allowedChars: z.string(),
  }),
  key: z.object({
    type: z.string(),
  }),
  ip: z.object({
    type: z.string(),
    allowedChars: z.string(),
  }),
  ssid: z.object({
    type: z.string(),
  }),
  gateway: z.object({
    type: z.string(),
    allowedChars: z.string(),
  }),
  region: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  priority: z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  nameservers: z.object({
    type: z.string(),
    allowedChars: z.string(),
  }),
});

export type WifiConfigOptions = z.infer<typeof WifiConfigOptionsSchema>
