/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const BiosListSchema = z.record(
  z.string(),
  z.object({
    fullName: z.string(),
    scanResult: z.object({
      total: z.number(),
      ok: z.number(),
      ko: z.number(),
      unsafe: z.number(),
      notFound: z.number(),
      hashOk: z.number(),
      hashKo: z.number(),
    }),
    biosList: z.record(
      z.string(),
      z.object({
        mandatory: z.boolean(),
        hashMatchingMandatory: z.boolean(),
        displayFileName: z.string(),
        notes: z.string(),
        currentMd5: z.string(),
        md5List: z.array(z.string()),
        cores: z.array(z.string()),
        lightStatus: z.string(),
        realStatus: z.string(),
      }),
    ),
  }),
);

export type BiosList = z.infer<typeof BiosListSchema>
