/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { EmulationStationConfig, EmulationStationConfigOptions } from 'stores/types/emulationstationConfig';

export type EmulationStationStoreState = {
  _baseUrl: string,
  _emulationstationOptions: EmulationStationConfigOptions,
  emulationstation: EmulationStationConfig,
};

export const useEmulationstationStore = defineStore('emulationstation', {
  state: () => ({
    _baseUrl: CONFIGURATION.emustation,
    _emulationstationOptions: {
      'videosnaps.loop': {
        lowerValue: 0,
        higherValue: 0,
      },
      'videosnaps.delay': {
        lowerValue: 0,
        higherValue: 0,
      },
      'screensaver.time': {
        lowerValue: 0,
        higherValue: 0,
      },
      'arcade.position': {
        lowerValue: -200,
        higherValue: 200,
      },
      brightness: {
        lowerValue: 0,
        higherValue: 8,
      },
    },
    emulationstation: {},
  } as EmulationStationStoreState),

  getters: {
    menuOptions: (state) => state._emulationstationOptions.menu.allowedStringList,
    selectedsystemOptions: (state) => state._emulationstationOptions.selectedsystem.allowedStringList,
    videosnapsLoopOptions: (state) => state._emulationstationOptions['videosnaps.loop'],
    videosnapsDelayOptions: (state) => state._emulationstationOptions['videosnaps.delay'],
    screensaverTimeOptions: (state) => state._emulationstationOptions['screensaver.time'],
    screensaverTypeOptions: (state) => state._emulationstationOptions['screensaver.type'].allowedStringList,
    themeFolderOptions: (state) => state._emulationstationOptions['theme.folder'].allowedStringList,
    arcadePositionOptions: (state) => state._emulationstationOptions['arcade.position'],
    systemSortingOptions: (state) => state._emulationstationOptions.systemsorting.allowedStringList,
    themeTransitionOptions: (state) => state._emulationstationOptions['theme.transition'].allowedStringList,
    brightnessOptions: (state) => state._emulationstationOptions.brightness,
  },
});
