/**
 * @author Pit64
 */
import { z } from 'zod';

export const HatConfigSchema = z.object({
  'wpaf.enabled': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'wpaf.board': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
});

export type HatConfig = z.infer<typeof HatConfigSchema>

export const HatConfigOptionsSchema = z.object({
  'wpaf.enabled': z.object({
    type: z.string(),
  }),
  'wpaf.board': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
});

export type HatConfigOptions = z.infer<typeof HatConfigOptionsSchema>
