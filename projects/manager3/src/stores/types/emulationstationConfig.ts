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
  'collection.multiplayer': z.object({
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
  'theme.folder': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'collection.tate': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  arcade: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'arcade.position': z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  'arcade.includeneogeo': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'arcade.hideoriginals': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'collection.ports.hide': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'collection.lightgun.hide': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  quicksystemselect: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  showhelp: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'popoup.help': z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  'popoup.music': z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  'popoup.netplay': z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  systemsorting: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'theme.carousel': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'theme.transition': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  brightness: z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  showhidden: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  showonlylatestversion: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  hidenogames: z.object({
    exist: z.boolean(),
    value: z.boolean(),
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
  'collection.multiplayer': z.object({
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
  'theme.folder': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'collection.tate': z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  arcade: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  'arcade.position': z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  'arcade.includeneogeo': z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  'arcade.hideoriginals': z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  'collection.ports.hide': z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  'collection.lightgun.hide': z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  quickselectsystem: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  showhelp: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  'popoup.help': z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  'popoup.music': z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  'popoup.netplay': z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  systemsorting: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'theme.carousel': z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  'theme.transition': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  brightness: z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  showhidden: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  showonlylatestversion: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
  hidenogames: z.object({
    type: z.string(),
    value: z.boolean(),
  }),
});

export type EmulationStationConfigOptions = z.infer<typeof EmulationStationConfigOptionsSchema>
