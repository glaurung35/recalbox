/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { EMUSTATION } from 'src/router/api.routes';
import { EmulationStationConfig, EmulationStationConfigOptions } from 'stores/types/emulationstationConfig';

export type EmulationStationStoreState = {
  _baseUrl: string,
  _emulationstationOptions: EmulationStationConfigOptions,
  emulationstation: EmulationStationConfig,
};

export const useEmulationstationStore = defineStore('emulationstation', {
  state: () => ({
    _baseUrl: EMUSTATION,
    _emulationstationOptions: {
      'videosnaps.loop': {
        lowerValue: 0,
        higherValue: 0,
      },
      'videosnaps.delay': {
        lowerValue: 0,
        higherValue: 0,
      },
    },
    emulationstation: {},
  } as EmulationStationStoreState),

  getters: {
    menuOptions: (state) => state._emulationstationOptions.menu.allowedStringList,
    selectedsystemOptions: (state) => state._emulationstationOptions
      .selectedsystem.allowedStringList,
    videosnapsLoopOptions: (state) => state._emulationstationOptions['videosnaps.loop'],
    videosnapsDelayOptions: (state) => state._emulationstationOptions['videosnaps.delay'],
  },
});
