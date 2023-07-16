/**
 * @author Pit64
 */
import { z } from 'zod';

export const PatronConfigSchema = z.object({
  privatekey: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
});

export type PatronConfig = z.infer<typeof PatronConfigSchema>

export const PatronConfigOptionsSchema = z.object({
  privatekey: z.object({
    type: z.string(),
    allowedChars: z.string(),
  }),
});

export type PatronConfigOptions = z.infer<typeof PatronConfigOptionsSchema>
