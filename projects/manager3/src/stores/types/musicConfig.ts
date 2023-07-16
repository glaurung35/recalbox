/**
 * @author Pit64
 */
import { z } from 'zod';

export const MusicConfigSchema = z.object({
  'remoteplaylist.enable': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
});

export type MusicConfig = z.infer<typeof MusicConfigSchema>

export const MusicConfigOptionsSchema = z.object({
  'remoteplaylist.enable': z.object({
    type: z.string(),
  }),
});

export type MusicConfigOptions = z.infer<typeof MusicConfigOptionsSchema>
