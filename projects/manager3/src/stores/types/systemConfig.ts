/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const SystemConfigSchema = z.object({
  'emulators.specialkeys': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'splash.length': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'security.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  hostname: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'virtual-gamepads.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'ssh.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  kblayout: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  language: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'samba.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  timezone: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'manager.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'es.videomode': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
});

export type SystemConfig = z.infer<typeof SystemConfigSchema>

export const SystemConfigOptionsSchema = z.object({
  'emulators.specialkeys': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'splash.length': z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  'security.enabled': z.object({
    type: z.string(),
  }),
  hostname: z.object({
    type: z.string(),
    allowedChars: z.string(),
  }),
  'virtual-gamepads.enabled': z.object({
    type: z.string(),
  }),
  'ssh.enabled': z.object({
    type: z.string(),
  }),
  kblayout: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
    displayableStringList: z.array(z.string()),
  }),
  language: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
    displayableStringList: z.array(z.string()),
  }),
  'samba.enabled': z.object({
    type: z.string(),
  }),
  timezone: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'manager.enabled': z.object({
    type: z.string(),
  }),
  'es.videomode': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
});

export type SystemConfigOptions = z.infer<typeof SystemConfigOptionsSchema>
