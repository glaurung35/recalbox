/**
 * @author Pit64
 */
import { z } from 'zod';

export const PatreonConfigSchema = z.object({
  privatekey: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
});

export type PatreonConfig = z.infer<typeof PatreonConfigSchema>

export const PatreonConfigOptionsSchema = z.object({
  privatekey: z.object({
    type: z.string(),
    allowedChars: z.string(),
  }),
});

export type PatreonConfigOptions = z.infer<typeof PatreonConfigOptionsSchema>
