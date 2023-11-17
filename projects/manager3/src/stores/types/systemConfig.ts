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
  'splash.select': z.object({
    exist: z.boolean(),
    value: z.string(),
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
  'power.switch': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'fbcp.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'api.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  overscan: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'secondminitft.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'secondminitft.type': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'secondminitft.resolution': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'secondminitft.imagestretchenabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'secondminitft.imageenlargeenabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'secondminitft.imagealphaenabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'secondminitft.imageignoreaspectenabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'secondminitft.disablevideoines': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'secondminitft.backlightcontrol': z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  'secondminitft.usemarquee': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'secondminitft.sleepenabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'externalscreen.prefered': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'externalscreen.forceresolution': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'externalscreen.forcefrequency': z.object({
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
  'splash.select': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
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
    displayableStringList: z.array(z.string()),
  }),
  'power.switch': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'fbcp.enabled': z.object({
    type: z.string(),
  }),
  'api.enabled': z.object({
    type: z.string(),
  }),
  overscan: z.object({
    type: z.string(),
  }),
  'secondminitft.enabled': z.object({
    type: z.string(),
  }),
  'secondminitft.type': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'secondminitft.resolution': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'secondminitft.imagestretchenabled': z.object({
    type: z.string(),
  }),
  'secondminitft.imageenlargeenabled': z.object({
    type: z.string(),
  }),
  'secondminitft.imagealphaenabled': z.object({
    type: z.string(),
  }),
  'secondminitft.imageignoreaspectenabled': z.object({
    type: z.string(),
  }),
  'secondminitft.disablevideoines': z.object({
    type: z.string(),
  }),
  'secondminitft.backlightcontrol': z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  'secondminitft.usemarquee': z.object({
    type: z.string(),
  }),
  'secondminitft.sleepenabled': z.object({
    type: z.string(),
  }),
  'externalscreen.prefered': z.object({
    type: z.string(),
    allowedChars: z.string(),
  }),
  'externalscreen.forceresolution': z.object({
    type: z.string(),
    allowedChars: z.string(),
  }),
  'externalscreen.forcefrequency': z.object({
    type: z.string(),
    allowedChars: z.string(),
  }),
});

export type SystemConfigOptions = z.infer<typeof SystemConfigOptionsSchema>
