/**
 * @author Pit64
 */
import { z } from 'zod';

export const Wifi2ConfigSchema = z.object({
  key: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  ssid: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
});

export type Wifi2Config = z.infer<typeof Wifi2ConfigSchema>

export const Wifi2ConfigOptionsSchema = z.object({
  key: z.object({
    type: z.string(),
  }),
  ssid: z.object({
    type: z.string(),
  }),
});

export type Wifi2ConfigOptions = z.infer<typeof Wifi2ConfigOptionsSchema>
