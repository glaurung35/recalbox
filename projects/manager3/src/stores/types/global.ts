/**
 * @author Nicolas TESSIER aka Asthonishia
 */

export interface GlobalConfigResponse {
  'retroachievements.username': {
    exist: boolean;
    value: string;
  };
  autosave: {
    exist: boolean;
    value: boolean;
  };
  netplay: {
    exist: boolean;
    value: boolean;
  };
  'netplay.nickname': {
    exist: boolean;
    value: string;
  };
  'demo.duration': {
    exist: boolean;
    value: number;
  };
  shaderset: {
    exist: boolean;
    value: string;
  };
  'netplay.lobby': {
    exist: boolean;
    value: string;
  };
  'translate.url': {
    exist: boolean;
    value: string;
  };
  'shaderset.file': {
    exist: boolean;
    value: string;
  };
  'translate.apikey': {
    exist: boolean;
    value: string;
  };
  'demo.infoscreenduration': {
    exist: boolean;
    value: number;
  };
  shaders: {
    exist: boolean;
    value: boolean;
  };
  retroachievements: {
    exist: boolean;
    value: boolean;
  };
  'retroachievements.password': {
    exist: boolean;
    value: string;
  };
  'translate.from': {
    exist: boolean;
    value: string;
  };
  videomode: {
    exist: boolean;
    value: string;
  };
  translate: {
    exist: boolean;
    value: boolean;
  };
  'retroachievements.hardcore': {
    exist: boolean;
    value: boolean;
  };
  ratio: {
    exist: boolean;
    value: string;
  };
  smooth: {
    exist: boolean;
    value: boolean;
  };
  'demo.systemlist': {
    exist: boolean;
    value: string[];
  };
  'netplay.relay': {
    exist: boolean;
    value: string;
  };
  rewind: {
    exist: boolean;
    value: boolean;
  };
  inputdriver: {
    exist: boolean;
    value: string;
  };
  integerscale: {
    exist: boolean;
    value: boolean;
  };
  'translate.to': {
    exist: boolean;
    value: string;
  };
  'netplay.port': {
    exist: boolean;
    value: number;
  };
  'show.savestate.before.run': {
    exist: boolean;
    value: boolean;
  };
  softpatching: {
    exist: boolean;
    value: string[];
  };
  'netplay.active': {
    exist: boolean;
    value: boolean;
  };
  recalboxoverlays: {
    exist: boolean;
    value: boolean;
  };
  showfps: {
    exist: boolean;
    value: boolean;
  };
  hidepreinstalledgames: {
    exist: boolean;
    value: boolean;
  };
  quitpresstwice: {
    exist: boolean;
    value: boolean;
  };
  hdmode: {
    exist: boolean;
    value: boolean;
  };
  widescreenmode: {
    exist: boolean;
    value: boolean;
  };
  reducelatency: {
    exist: boolean;
    value: boolean;
  };
  runahead: {
    exist: boolean;
    value: boolean;
  }
}

export interface GlobalConfigOptionsResponse {
  'retroachievements.username': {
    type: string;
  };
  autosave: {
    type: string;
  };
  netplay: {
    type: string;
  };
  'netplay.nickname': {
    type: string;
    allowedChars: string;
  };
  'demo.duration': {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  shaderset: {
    type: string;
    allowedStringList: string[];
  };
  'netplay.lobby': {
    type: string;
  };
  'translate.url': {
    type: string;
  };
  'shaderset.file': {
    type: string;
    allowedStringList: string[];
  };
  'translate.apikey': {
    type: string;
  };
  'demo.infoscreenduration': {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  shaders: {
    type: string;
  };
  retroachievements: {
    type: string;
  };
  'retroachievements.password': {
    type: string;
  };
  'translate.from': {
    type: string;
    allowedStringList: string[];
  };
  videomode: {
    type: string;
    allowedStringList: string[];
    displayableStringList: string[];
  };
  translate: {
    type: string;
  };
  'retroachievements.hardcore': {
    type: string;
  };
  ratio: {
    type: string;
    allowedStringList: string[];
  };
  smooth: {
    type: string;
  };
  'demo.systemlist': {
    type: string;
    allowedStringList: string[];
  };
  'netplay.relay': {
    type: string;
    allowedStringList: string[];
  };
  rewind: {
    type: string;
  };
  inputdriver: {
    type: string;
    allowedStringList: string[];
  };
  integerscale: {
    type: string;
  };
  'translate.to': {
    type: string;
    allowedStringList: string[];
  };
  'netplay.port': {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  'show.savestate.before.run': {
    type: string;
  };
  softpatching: {
    type: string;
    allowedStringList: string[];
  };
  configfile: {
    type: string;
  };
  'netplay.active': {
    type: string;
  };
  recalboxoverlays: {
    type: string;
  };
  showfps: {
    type: string;
  };
  hidepreinstalledgames: {
    type: string;
  };
  quitpresstwice: {
    type: string;
  };
  hdmode: {
    type: string;
  };
  widescreenmode: {
    type: string;
  };
  reducelatency: {
    type: string
  };
  runahead: {
    type: string;
  };
}
