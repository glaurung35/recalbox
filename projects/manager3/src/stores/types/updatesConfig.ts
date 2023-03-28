import { z } from 'zod';

export const UpdatesConfigSchema = z.object({
  type: z.object({
    type: z.boolean(),
    value: z.string(),
  }),
  'review.key': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  enabled: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
});

export type UpdatesConfig = z.infer<typeof UpdatesConfigSchema>

export const UpdatesConfigOptionsSchema = z.object({
  type: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'review.key': z.object({
    type: z.string(),
  }),
  enabled: z.object({
    type: z.string(),
  }),
});

export type UpdatesConfigOptions = z.infer<typeof UpdatesConfigOptionsSchema>
