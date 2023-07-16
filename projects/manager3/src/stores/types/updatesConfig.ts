/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const UpdatesConfigSchema = z.object({
  enabled: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
});

export type UpdatesConfig = z.infer<typeof UpdatesConfigSchema>

export const UpdatesConfigOptionsSchema = z.object({
  enabled: z.object({
    type: z.string(),
  }),
});

export type UpdatesConfigOptions = z.infer<typeof UpdatesConfigOptionsSchema>
