/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { z } from 'zod';

export const GlobalConfigSchema = z.object({
  'retroachievements.username': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  autosave: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  netplay: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'netplay.nickname': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'demo.duration': z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  shaderset: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'netplay.lobby': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'translate.url': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'shaderset.file': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'translate.apikey': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'demo.infoscreenduration': z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  shaders: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  retroachievements: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'retroachievements.password': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'translate.from': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  videomode: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  translate: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'retroachievements.hardcore': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  ratio: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  smooth: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'demo.systemlist': z.object({
    exist: z.boolean(),
    value: z.array(z.string()),
  }),
  'netplay.relay': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  rewind: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  inputdriver: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  integerscale: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  'translate.to': z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'netplay.port': z.object({
    exist: z.boolean(),
    value: z.number(),
  }),
  'show.savestate.before.run': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  softpatching: z.object({
    exist: z.boolean(),
    value: z.array(z.string()),
  }),
  configfile: z.object({
    exist: z.boolean(),
    value: z.string(),
  }),
  'netplay.active': z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  recalboxoverlays: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  showfps: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  hidepreinstalledgames: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
  quitpresstwice: z.object({
    exist: z.boolean(),
    value: z.boolean(),
  }),
});

export type GlobalConfig = z.infer<typeof GlobalConfigSchema>

export const GlobalConfigOptionsSchema = z.object({
  'retroachievements.username': z.object({
    type: z.string(),
  }),
  autosave: z.object({
    type: z.string(),
  }),
  netplay: z.object({
    type: z.string(),
  }),
  'netplay.nickname': z.object({
    type: z.string(),
  }),
  'demo.duration': z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  shaderset: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'netplay.lobby': z.object({
    type: z.string(),
  }),
  'translate.url': z.object({
    type: z.string(),
  }),
  'shaderset.file': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'translate.apikey': z.object({
    type: z.string(),
  }),
  'demo.infoscreenduration': z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  shaders: z.object({
    type: z.string(),
  }),
  retroachievements: z.object({
    type: z.string(),
  }),
  'retroachievements.password': z.object({
    type: z.string(),
  }),
  'translate.from': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  videomode: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
    displayableStringList: z.array(z.string()),
  }),
  translate: z.object({
    type: z.string(),
  }),
  'retroachievements.hardcore': z.object({
    type: z.string(),
  }),
  ratio: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  smooth: z.object({
    type: z.string(),
  }),
  'demo.systemlist': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'netplay.relay': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  rewind: z.object({
    type: z.string(),
  }),
  inputdriver: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  integerscale: z.object({
    type: z.string(),
  }),
  'translate.to': z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  'netplay.port': z.object({
    type: z.string(),
    lowerValue: z.number(),
    higherValue: z.number(),
  }),
  'show.savestate.before.run': z.object({
    type: z.string(),
  }),
  softpatching: z.object({
    type: z.string(),
    allowedStringList: z.array(z.string()),
  }),
  configfile: z.object({
    type: z.string(),
  }),
  'netplay.active': z.object({
    type: z.string(),
  }),
  recalboxoverlays: z.object({
    type: z.string(),
  }),
  showfps: z.object({
    type: z.string(),
  }),
  hidepreinstalledgames: z.object({
    type: z.string(),
  }),
  quitpresstwice: z.object({
    type: z.string(),
  }),
});

export type GlobalConfigOptions = z.infer<typeof GlobalConfigOptionsSchema>
