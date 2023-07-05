/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const EmulationStationConfigSchema = z.object({
  bootongamelist: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  filteradultgames: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  gamelistonly: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  forcebasicgamelistview: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  menu: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'collection.lastplayed': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'videosnaps.loop': z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  selectedsystem: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'collection.multiplayers': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'collection.allgames': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'videosnaps.delay': z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  hidesystemview: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  clock: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  favoritesonly: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  showgameclipclippingitem: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  showgamecliphelpitems: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'screensaver.time': z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  'screensaver.type': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
});

export type EmulationStationConfig = z.infer<typeof EmulationStationConfigSchema>

export const EmulationStationConfigOptionsSchema = z.object({
  bootongamelist: z.object({
    type: z.string(),
  }),
  filteradultgames: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  gamelistonly: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  forcebasicgamelistview: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  menu: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'collection.lastplayed': z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  'videosnaps.loop': z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  selectedsystem: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'collection.multiplayers': z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  'collection.allgames': z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  'videosnaps.delay': z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  hidesystemview: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  clock: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  favoritesonly: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  showgameclipclippingitem: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  showgamecliphelpitems: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  'screensaver.time': z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  'screensaver.type': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
});

export type EmulationStationConfigOptions = z.infer<typeof EmulationStationConfigOptionsSchema>
