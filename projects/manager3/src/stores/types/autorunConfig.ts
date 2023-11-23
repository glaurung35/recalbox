/**
 * @author Pit64
 */
import { z } from 'zod';

export const AutorunConfigSchema = z.object({
  enabled: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  systemuuid: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  gamepath: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
});

export type AutorunConfig = z.infer<typeof AutorunConfigSchema>

export const AutorunConfigOptionsSchema = z.object({
  enabled: z.object({
    type: z.string(),
  }),
  systemuuid: z.object({
    type: z.string(),
  }),
  gamepath: z.object({
    type: z.string(),
  }),
});

export type AutorunConfigOptions = z.infer<typeof AutorunConfigOptionsSchema>
