/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { SystemConfig, SystemConfigOptions } from 'stores/types/systemConfig';

export type SystemStoreState = {
  _baseUrl: string,
  _systemOptions: SystemConfigOptions,
  system: SystemConfig,
};

export const useSystemStore = defineStore('system', {
  state: () => ({
    _baseUrl: CONFIGURATION.system,
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
        lowerValue: -1,
        higherValue: 300,
      },
      'splash.select': {
        allowedStringList: [''],
      },
      'power.switch': {
        allowedStringList: [''],
      },
      'secondminitft.type': {
        allowedStringList: [''],
      },
      'secondminitft.resolution': {
        allowedStringList: [''],
      },
      'secondminitft.backlightcontrol': {
        lowerValue: 0,
        higherValue: 100,
      },
    },
    system: {
      language: {
        value: 'fr_FR',
      },
    },
  } as SystemStoreState),

  getters: {
    languageOptions: (state) => state._systemOptions.language.allowedStringList,
    kblayoutOptions: (state) => state._systemOptions.kblayout.allowedStringList,
    timezoneOptions: (state) => state._systemOptions.timezone.allowedStringList,
    specialkeysOptions: (state) => state._systemOptions['emulators.specialkeys'].allowedStringList,
    esVideomodeOptions: (state) => state._systemOptions['es.videomode'].allowedStringList,
    splashLengthOptions: (state) => state._systemOptions['splash.length'],
    splashSelectOptions: (state) => state._systemOptions['splash.select'].allowedStringList,
    powerSwitchOptions: (state) => state._systemOptions['power.switch'].allowedStringList,
    secondminitftTypeOptions: (state) => state._systemOptions['secondminitft.type'].allowedStringList,
    secondminitftResolutionOptions: (state) => state._systemOptions['secondminitft.resolution'].allowedStringList,
    secondminitftBacklightDurationOptions: (state) => state._systemOptions['secondminitft.backlightcontrol'],
  },
});
