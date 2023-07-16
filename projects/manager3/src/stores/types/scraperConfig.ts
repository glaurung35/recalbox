/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const ScraperConfigSchema = z.object({
  'screenscraper.password': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'screenscraper.media': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  extractregionfromfilename: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  getnamefrom: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'screenscraper.language': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'screenscraper.region': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'screenscraper.thumbnail': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'screenscraper.video': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'screenscraper.maps': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'screenscraper.manual': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'screenscraper.user': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  source: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  auto: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'screenscraper.p2k': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'screenscraper.regionPriority': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
});

export type ScraperConfig = z.infer<typeof ScraperConfigSchema>

export const ScraperConfigOptionsSchema = z.object({
  'screenscraper.password': z.object({
    type: z.string(),
  }),
  'screenscraper.media': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  extractregionfromfilename: z.object({
    type: z.string(),
  }),
  getnamefrom: z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  'screenscraper.language': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'screenscraper.region': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'screenscraper.thumbnail': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'screenscraper.video': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'screenscraper.maps': z.object({
    type: z.string(),
  }),
  'screenscraper.manual': z.object({
    type: z.string(),
  }),
  'screenscraper.user': z.object({
    type: z.string(),
  }),
  source: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  auto: z.object({
    type: z.string(),
  }),
  'screenscraper.p2k': z.object({
    type: z.string(),
  }),
  'screenscraper.regionPriority': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
});

export type ScraperConfigOptions = z.infer<typeof ScraperConfigOptionsSchema>
