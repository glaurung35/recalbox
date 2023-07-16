/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const VersionsListSchema = z.object({
  webapi: z.string(),
  recalbox: z.string(),
  linux: z.string(),
  libretro: z.object({
    retroarch: z.string(),
    cores: z.record(z.string(), z.string()),
  }),
});

export type VersionsList = z.infer<typeof VersionsListSchema>
