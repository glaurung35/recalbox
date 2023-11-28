/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import {
  SystemConfigOptionsResponse,
  SystemConfigResponse,
} from 'stores/types/system';

export type SystemStoreState = {
  _baseUrl: string,
  _systemOptions: SystemConfigOptionsResponse,
  system: SystemConfigResponse,
};

export const useSystemStore = defineStore('system', {
  state: () => ({
    _baseUrl: CONFIGURATION.system,
    _systemOptions: {
      language: {
        allowedStringList: [''],
        displayableStringList: [''],
      },
      kblayout: {
        allowedStringList: [''],
        displayableStringList: [''],
      },
      timezone: {
        allowedStringList: [''],
      },
      'emulators.specialkeys': {
        allowedStringList: [''],
      },
      'es.videomode': {
        allowedStringList: [''],
        displayableStringList: [''],
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
      'externalscreen.prefered': {
        allowedChars: '',
      },
      'externalscreen.forceresolution': {
        allowedChars: '',
      },
      'externalscreen.forcefrequency': {
        allowedChars: '',
      },
      hostname: {
        allowedChars: '',
      },
    },
    system: {
      language: {
        value: 'fr_FR',
      },
    },
  } as SystemStoreState),

  getters: {
    languageOptions: (state) => state._systemOptions.language.allowedStringList.sort(),
    kblayoutOptions: (state) => state._systemOptions.kblayout.allowedStringList.sort(),
    timezoneOptions: (state) => state._systemOptions.timezone.allowedStringList.sort(),
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
