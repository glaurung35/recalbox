/**
 * @author Pit64
 */
import { z } from 'zod';

export const Wifi3ConfigSchema = z.object({
  key: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  ssid: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
});

export type Wifi3Config = z.infer<typeof Wifi3ConfigSchema>

export const Wifi3ConfigOptionsSchema = z.object({
  key: z.object({
    type: z.string(),
  }),
  ssid: z.object({
    type: z.string(),
  }),
});

export type Wifi3ConfigOptions = z.infer<typeof Wifi3ConfigOptionsSchema>
