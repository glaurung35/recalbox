/**
 * @author Nicolas TESSIER aka Asthonishia
 */

export interface EmulationStationConfigResponse {
  bootongamelist: {
    exist: boolean;
    value: boolean;
  };
  filteradultgames: {
    exist: boolean;
    value: boolean;
  };
  gamelistonly: {
    exist: boolean;
    value: boolean;
  };
  forcebasicgamelistview: {
    exist: boolean;
    value: boolean;
  };
  menu: {
    exist: boolean;
    value: string;
  };
  'collection.lastplayed': {
    exist: boolean;
    value: boolean;
  };
  'videosnaps.loop': {
    exist: boolean;
    value: number;
  };
  selectedsystem: {
    exist: boolean;
    value: string;
  };
  'collection.multiplayer': {
    exist: boolean;
    value: boolean;
  };
  'collection.allgames': {
    exist: boolean;
    value: boolean;
  };
  'videosnaps.delay': {
    exist: boolean;
    value: number;
  };
  hidesystemview: {
    exist: boolean;
    value: boolean;
  };
  clock: {
    exist: boolean;
    value: boolean;
  };
  favoritesonly: {
    exist: boolean;
    value: boolean;
  };
  showgameclipclippingitem: {
    exist: boolean;
    value: boolean;
  };
  showgamecliphelpitems: {
    exist: boolean;
    value: boolean;
  };
  'screensaver.time': {
    exist: boolean;
    value: number;
  };
  'screensaver.type': {
    exist: boolean;
    value: string;
  };
  'theme.folder': {
    exist: boolean;
    value: string;
  };
  'collection.tate': {
    exist: boolean;
    value: boolean;
  };
  virtualarcade: {
    exist: boolean;
    value: boolean;
  };
  'virtualarcade.includeneogeo': {
    exist: boolean;
    value: boolean;
  };
  'virtualarcade.hideoriginals': {
    exist: boolean;
    value: boolean;
  };
  'collection.ports.hide': {
    exist: boolean;
    value: boolean;
  };
  'collection.lightgun.hide': {
    exist: boolean;
    value: boolean;
  };
  quicksystemselect: {
    exist: boolean;
    value: boolean;
  };
  showhelp: {
    exist: boolean;
    value: boolean;
  };
  'popoup.help': {
    exist: boolean;
    value: number;
  };
  'popoup.music': {
    exist: boolean;
    value: number;
  };
  'popoup.netplay': {
    exist: boolean;
    value: number;
  };
  systemsorting: {
    exist: boolean;
    value: string;
  };
  'theme.carousel': {
    exist: boolean;
    value: boolean;
  };
  'theme.transition': {
    exist: boolean;
    value: string;
  };
  brightness: {
    exist: boolean;
    value: number;
  };
  showhidden: {
    exist: boolean;
    value: boolean;
  };
  showonlylatestversion: {
    exist: boolean;
    value: boolean;
  };
  hidenogames: {
    exist: boolean;
    value: boolean;
  };
  'arcade.view.enhanced': {
    exist: boolean;
    value: boolean;
  };
  'arcade.view.hideclones': {
    exist: boolean;
    value: boolean;
  };
  'arcade.view.hidebios': {
    exist: boolean;
    value: boolean;
  };
  'arcade.view.hidenonworking': {
    exist: boolean;
    value: boolean;
  };
  'arcade.usedatabasenames': {
    exist: boolean;
    value: boolean;
  };
  'virtualarcade.manufacturers': {
    exist: boolean;
    value: string[];
  };
  debuglogs: {
    exist: boolean;
    value: boolean;
  };
  'pads.osd': {
    exist: boolean;
    value: boolean;
  };
  'pads.osd.type': {
    exist: boolean;
    value: string;
  };
}

export interface EmulationStationConfigOptionsResponse {
  bootongamelist: {
    type: string;
  };
  filteradultgames: {
    type: string;
    value: boolean;
  };
  gamelistonly: {
    type: string;
    value: boolean;
  };
  forcebasicgamelistview: {
    type: string;
    value: boolean;
  };
  menu: {
    type: string;
    allowedStringList: string[];
  };
  'collection.lastplayed': {
    type: string;
    value: boolean;
  };
  'videosnaps.loop': {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  selectedsystem: {
    type: string;
    allowedStringList: string[];
  };
  'collection.multiplayer': {
    type: string;
    value: boolean;
  };
  'collection.allgames': {
    type: string;
    value: boolean;
  };
  'videosnaps.delay': {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  hidesystemview: {
    type: string;
    value: boolean;
  };
  clock: {
    type: string;
    value: boolean;
  };
  favoritesonly: {
    type: string;
    value: boolean;
  };
  showgameclipclippingitem: {
    type: string;
    value: boolean;
  };
  showgamecliphelpitems: {
    type: string;
    value: boolean;
  };
  'screensaver.time': {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  'screensaver.type': {
    type: string;
    allowedStringList: string[];
  };
  'theme.folder': {
    type: string;
    allowedStringList: string[];
  };
  'collection.tate': {
    type: string;
    value: boolean;
  };
  virtualarcade: {
    type: string;
    value: boolean;
  };
  'virtualarcade.includeneogeo': {
    type: string;
    value: boolean;
  };
  'virtualarcade.hideoriginals': {
    type: string;
    value: boolean;
  };
  'collection.ports.hide': {
    type: string;
    value: boolean;
  };
  'collection.lightgun.hide': {
    type: string;
    value: boolean;
  };
  quickselectsystem: {
    type: string;
    value: boolean;
  };
  showhelp: {
    type: string;
    value: boolean;
  };
  'popoup.help': {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  'popoup.music': {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  'popoup.netplay': {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  systemsorting: {
    type: string;
    allowedStringList: string[];
  };
  'theme.carousel': {
    type: string;
    value: boolean;
  };
  'theme.transition': {
    type: string;
    allowedStringList: string[];
  };
  brightness: {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  showhidden: {
    type: string;
    value: boolean;
  };
  showonlylatestversion: {
    type: string;
    value: boolean;
  };
  hidenogames: {
    type: string;
    value: boolean;
  };
  'arcade.view.enhanced': {
    type: string;
    value: boolean;
  };
  'arcade.view.hideclones': {
    type: string;
    value: boolean;
  };
  'arcade.view.hidebios': {
    type: string;
    value: boolean;
  };
  'arcade.view.hidenonworking': {
    type: string;
    value: boolean;
  };
  'arcade.usedatabasenames': {
    type: string;
    value: boolean;
  };
  'virtualarcade.manufacturers': {
    type: string;
    allowedStringList: string[];
    displayableStringList: string[];
  };
  debuglogs: {
    type: string;
    value: boolean;
  };
  'pads.osd': {
    type: string;
    value: boolean;
  };
  'pads.osd.type': {
    type: string;
    allowedStringList: string[];
    displayableStringList: string[];
  };
}

export interface CurrentSystemMetaDataColor {
  band1: string;
  band2: string;
  band3: string;
  band4: string;
}

export interface CurrentSystemMetaData {
  system: object;
  colors: CurrentSystemMetaDataColor
}

export interface CurrentSystem {
  name: string;
  systemId: string;
  logoPath: string;
  consolePath: string;
  gamePath: string;
  metaData: CurrentSystemMetaData;
}

export interface CurrentRom {
  name: string;
  imagePath: string;
  thumbnailPath: string;
  videoPath: string;
  developer: string;
  publisher: string;
  players: string;
  region: string;
  genre: string;
}

export interface EmulationStationCurrentState {
  currentSystem: CurrentSystem|null,
  currentRom: CurrentRom|null,
}
