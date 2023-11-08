/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const StoragesListSchema = z.object({
  storages: z.record(
    z.string(),
    z.object({
      recalbox: z.string(),
      devicebus: z.string(),
      devicetype: z.string(),
      devicemodel: z.string(),
      mount: z.string(),
      filesystem: z.string(),
      filesystemtype: z.string(),
      size: z.number(),
      used: z.number(),
    }),
  ),
});

export type StoragesList = z.infer<typeof StoragesListSchema>
