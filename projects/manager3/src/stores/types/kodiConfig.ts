/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const KodiConfigSchema = z.object({
  enabled: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  xbutton: z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  'network.waittime': z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  'network.waithost': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  atstartup: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  videomode: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'network.waitmode': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
});

export type KodiConfig = z.infer<typeof KodiConfigSchema>

export const KodiConfigOptionsSchema = z.object({
  enabled: z.object({
    type: z.string(),
  }),
  xbutton: z.object({
    type: z.string(),
    value: z.number(),
  }),
  'network.waittime': z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  'network.waithost': z.object({
    type: z.string(),
    allowedChars: z.string(),
  }),
  atstartup: z.object({
    type: z.string(),
  }),
  videomode: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
    displayableStringList: z.array(z.string()),
  }),
  'network.waitmode': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
});

export type KodiConfigOptions = z.infer<typeof KodiConfigOptionsSchema>
