/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const SystemsListSchema = z.object({
  romPath: z.string(),
  systemList: z.record(
    z.string(),
    z.object({
      name: z.string(),
      fullname: z.string(),
      romFolder: z.string(),
      themeFolder: z.string(),
      extensions: z.array(z.string()),
      command: z.string(),
      emulators: z.object({
        libretro: z.record(
          z.string(),
          z.string(),
        ),
      }),
    }),
  ),
});

export type SystemsList = z.infer<typeof SystemsListSchema>
