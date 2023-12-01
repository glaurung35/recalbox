/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import {
  EmulationStationConfigOptionsResponse,
  EmulationStationConfigResponse, EmulationStationCurrentState,
} from 'stores/types/emulationstation';

export interface EmulationStationStoreState extends FetchStore, PostStore, FetchOptionsStore {
  _baseUrl: string;
  _emulationstationOptions: EmulationStationConfigOptionsResponse;
  emulationstation: EmulationStationConfigResponse;
  currentState: EmulationStationCurrentState;
}

export const useEmulationstationStore = defineStore('emulationstation', {
  state: () => ({
    _baseUrl: CONFIGURATION.emustation,
    _emulationstationOptions: {
      'videosnaps.loop': {
        lowerValue: 0,
        higherValue: 300,
      },
      'videosnaps.delay': {
        lowerValue: 0,
        higherValue: 15000,
      },
      'screensaver.type': {
        allowedStringList: [''],
      },
      'screensaver.time': {
        lowerValue: 0,
        higherValue: 30,
      },
      brightness: {
        lowerValue: 0,
        higherValue: 8,
      },
      'popoup.help': {
        lowerValue: 0,
        higherValue: 10,
      },
      'popoup.music': {
        lowerValue: 0,
        higherValue: 10,
      },
      'popoup.netplay': {
        lowerValue: 0,
        higherValue: 10,
      },
      systemsorting: {
        allowedStringList: [''],
      },
      'virtualarcade.manufacturers': {
        allowedStringList: [''],
        displayableStringList: [''],
      },
      'pads.osd.type': {
        allowedStringList: [''],
        displayableStringList: [''],
      },
      menu: {
        allowedStringList: [''],
      },
      selectedsystem: {
        allowedStringList: [''],
      },
      'theme.folder': {
        allowedStringList: [''],
      },
      'theme.transition': {
        allowedStringList: [''],
      },
    },
    emulationstation: {},
    currentState: {},
  } as EmulationStationStoreState),

  getters: {
    menuOptions: (state) => state._emulationstationOptions.menu.allowedStringList,
    selectedsystemOptions: (state) => state._emulationstationOptions.selectedsystem.allowedStringList,
    videosnapsLoopOptions: (state) => state._emulationstationOptions['videosnaps.loop'],
    videosnapsDelayOptions: (state) => state._emulationstationOptions['videosnaps.delay'],
    screensaverTimeOptions: (state) => state._emulationstationOptions['screensaver.time'],
    screensaverTypeOptions: (state) => state._emulationstationOptions['screensaver.type'].allowedStringList,
    themeFolderOptions: (state) => state._emulationstationOptions['theme.folder'].allowedStringList,
    systemSortingOptions: (state) => state._emulationstationOptions.systemsorting.allowedStringList,
    themeTransitionOptions: (state) => state._emulationstationOptions['theme.transition'].allowedStringList,
    brightnessOptions: (state) => state._emulationstationOptions.brightness,
    popupHelpOptions: (state) => state._emulationstationOptions['popoup.help'],
    popupMusicOptions: (state) => state._emulationstationOptions['popoup.music'],
    popupNetplayOptions: (state) => state._emulationstationOptions['popoup.netplay'],
    systemsortingOptions: (state) => state._emulationstationOptions.systemsorting.allowedStringList,
    virtualArcadeManufacturersOptions: (state) => state._emulationstationOptions['virtualarcade.manufacturers'].allowedStringList,
    padsOsdTypeOptions: (state) => state._emulationstationOptions['pads.osd.type'].allowedStringList,
  },

  actions: {
    resetCurrentSystem() {
      if (this.currentState.currentSystem) {
        this.currentState.currentSystem = null;
      }
    },
  },
});
