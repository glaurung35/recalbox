import { defineStore } from 'pinia';
import { SYSTEM } from 'src/router/api.routes';
import { SystemConfig, SystemConfigOptions } from 'stores/types/systemConfig';

export type SystemStoreState = {
  _baseUrl: string,
  _systemOptions: SystemConfigOptions,
  system: SystemConfig,
};

export const useSystemStore = defineStore('system', {
  state: () => ({
    _baseUrl: SYSTEM,
    _systemOptions: {
      language: {
        allowedStringList: [''],
      },
      kblayout: {
        allowedStringList: [''],
      },
      timezone: {
        allowedStringList: [''],
      },
      'emulators.specialkeys': {
        allowedStringList: [''],
      },
      'es.videomode': {
        allowedStringList: [''],
      },
      'splash.length': {
        lowerValue: 0,
        higherValue: 0,
      },
    },
    system: {},
  } as SystemStoreState),

  getters: {
    languageOptions: (state) => state._systemOptions.language.allowedStringList,
    kblayoutOptions: (state) => state._systemOptions.kblayout.allowedStringList,
    timezoneOptions: (state) => state._systemOptions.timezone.allowedStringList,
    specialkeysOptions: (state) => state._systemOptions['emulators.specialkeys'].allowedStringList,
    esVideomodeOptions: (state) => state._systemOptions['es.videomode'].allowedStringList,
    splashLengthOptions: (state) => state._systemOptions['splash.length'],
  },
});
