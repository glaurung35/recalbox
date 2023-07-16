/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const AudioConfigSchema = z.object({
  bgmusic: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  volume: z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  device: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  mode: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
});

export type AudioConfig = z.infer<typeof AudioConfigSchema>

export const AudioConfigOptionsSchema = z.object({
  bgmusic: z.object({
    type: z.string(),
  }),
  volume: z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  device: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  mode: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
});

export type AudioConfigOptions = z.infer<typeof AudioConfigOptionsSchema>
