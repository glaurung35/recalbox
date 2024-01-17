/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import {
  GlobalConfigOptionsResponse,
  GlobalConfigResponse,
} from 'stores/types/global';

export interface GlobalStoreState extends FetchStore, PostStore, FetchOptionsStore {
  _baseUrl: string;
  _globalOptions: GlobalConfigOptionsResponse;
  global: GlobalConfigResponse;
}

export const useGlobalStore = defineStore('global', {
  state: () => ({
    _baseUrl: CONFIGURATION.global,
    _globalOptions: {
      ratio: {
        allowedStringList: [''],
      },
      shaderset: {
        allowedStringList: [''],
      },
      'shaderset.file': {
        allowedStringList: [''],
      },
      'translate.from': {
        allowedStringList: [''],
      },
      'translate.to': {
        allowedStringList: [''],
      },
      videomode: {
        allowedStringList: [''],
        displayableStringList: [''],
      },
      'demo.infoscreenduration': {
        lowerValue: 5,
        higherValue: 30,
      },
      'demo.duration': {
        lowerValue: 30,
        higherValue: 600,
      },
      'demo.systemlist': {
        allowedStringList: [''],
      },
      'netplay.relay': {
        allowedStringList: [''],
      },
      softpatching: {
        allowedStringList: [''],
        displayableStringList: [''],
      },
      inputdriver: {
        allowedStringList: [''],
      },
      'netplay.port': {
        lowerValue: 1,
        higherValue: 65535,
      },
      'netplay.nickname': {
        allowedChars: '',
      },
    },
    global: {
      recalboxoverlays: {
        value: true,
      },
      videomode: {
        value: '',
      },
      shaderset: {
        value: 'none',
      },
      integerscale: {
        value: false,
      },
      shaders: {
        value: '',
      },
      ratio: {
        value: 'auto',
      },
      smooth: {
        value: true,
      },
      rewind: {
        value: true,
      },
      autosave: {
        value: false,
      },
      retroachievements: {
        value: false,
      },
      'retroachievements.hardcore': {
        value: false,
      },
      'retroachievements.username': {
        value: '',
      },
      'retroachievements.password': {
        value: '',
      },
      inputdriver: {
        value: 'auto',
      },
      'demo.systemlist': {
        value: '3do,amigacd32,atari2600,atari5200,atari7800,daphne,fbneo,fds,gamegear,gba,lynx,mame,mastersystem,megadrive,neogeo,nes,ngpc,pcengine,sega32x,sg1000,snes',
      },
      translate: {
        value: true,
      },
      'translate.from': {
        value: 'auto',
      },
      'translate.to': {
        value: 'auto',
      },
      netplay: {
        value: true,
      },
      'netplay.nickname': {
        value: '',
      },
      'netplay.port': {
        value: 55435,
      },
      'netplay.relay': {
        value: 'none',
      },
      'netplay.lobby': {
        value: 'http://lobby.libretro.com/list/',
      },
      quitpresstwice: {
        value: false,
      },
      hidepreinstalledgames: {
        value: false,
      },
      softpatching: {
        value: 'disable',
      },
      showfps: {
        value: false,
      },
      'demo.duration': {
        value: 90,
      },
      'demo.infoscreenduration': {
        value: 6,
      },
      reducelatency: {
        value: false,
      },
      runahead: {
        value: false,
      },
      'show.savestate.before.run': {
        value: false,
      },
      hdmode: {
        value: false,
      },
      widescreenmode: {
        value: false,
      },
    },
  } as GlobalStoreState),

  getters: {
    ratioOptions: (state) => state._globalOptions.ratio.allowedStringList.sort(),
    shadersetOptions: (state) => state._globalOptions.shaderset.allowedStringList.sort(),
    shadersetfileOptions: (state) => state._globalOptions['shaderset.file'].allowedStringList.sort(),
    translateFromOptions: (state) => state._globalOptions['translate.from'].allowedStringList.sort(),
    translateToOptions: (state) => state._globalOptions['translate.to'].allowedStringList.sort(),
    videomodeOptions: (state) => state._globalOptions.videomode.allowedStringList.sort(),
    demoInfoscreendurationOptions: (state) => state._globalOptions['demo.infoscreenduration'],
    demoDurationOptions: (state) => state._globalOptions['demo.duration'],
    demoSystemlistOptions: (state) => state._globalOptions['demo.systemlist'].allowedStringList.sort(),
    netplayRelayOptions: (state) => state._globalOptions['netplay.relay'].allowedStringList.sort(),
    softpatchingOptions: (state) => state._globalOptions.softpatching.allowedStringList.sort(),
    inputdriverOptions: (state) => state._globalOptions.inputdriver.allowedStringList.sort(),
  },
});
