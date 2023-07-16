/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const HyperionConfigSchema = z.object({
  enabled: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
});

export type HyperionConfig = z.infer<typeof HyperionConfigSchema>
