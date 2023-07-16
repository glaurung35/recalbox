/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const MediasListSchema = z.object({
  mediaPath: z.string(),
  mediaList: z.record(
    z.string(),
    z.object({
      type: z.string(),
    }),
  ),
});

export type MediasList = z.infer<typeof MediasListSchema>
