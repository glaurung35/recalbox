/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { GlobalConfig, GlobalConfigOptions } from 'stores/types/globalConfig';

export type GlobalStoreState = {
  _baseUrl: string,
  _globalOptions: GlobalConfigOptions,
  global: GlobalConfig,
};

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
      },
      'demo.infoscreenduration': {
        lowerValue: 0,
        higherValue: 0,
      },
      'demo.duration': {
        lowerValue: 0,
        higherValue: 0,
      },
      'demo.systemlist': {
        allowedStringList: [''],
      },
    },
    global: {},
  } as GlobalStoreState),

  getters: {
    ratioOptions: (state) => state._globalOptions.ratio.allowedStringList,
    shadersetOptions: (state) => state._globalOptions.shaderset.allowedStringList,
    shadersetfileOptions: (state) => state._globalOptions['shaderset.file'].allowedStringList,
    translateFromOptions: (state) => state._globalOptions['translate.from'].allowedStringList,
    translateToOptions: (state) => state._globalOptions['translate.to'].allowedStringList,
    videomodeOptions: (state) => state._globalOptions.videomode.allowedStringList,
    demoInfoscreendurationOptions: (state) => state._globalOptions['demo.infoscreenduration'],
    demoDurationOptions: (state) => state._globalOptions['demo.duration'],
    demoSystemlistOptions: (state) => state._globalOptions['demo.systemlist'].allowedStringList,
  },
});
