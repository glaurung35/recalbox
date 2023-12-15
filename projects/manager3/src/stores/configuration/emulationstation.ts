/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { apiUrl } from 'boot/axios';
import { defineStore } from 'pinia';
import {
  CONFIGURATION,
  getPath,
  GLOBAL,
  SYSTEMS,
  THEMES,
} from 'src/router/api.routes';
import { systemsMetaData } from 'src/utils/systemsMetaData';
import { ApiProviderStore } from 'stores/plugins/apiProviderStorePlugin';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import { useSystemsStore } from 'stores/systems';
import {
  EmulationStationConfigOptionsResponse,
  EmulationStationConfigResponse,
  EmulationStationCurrentState,
  RomMetaData,
} from 'stores/types/emulationstation';
import { Actions, EventResponse } from 'stores/types/mqtt';

const api: string|undefined = apiUrl;

export interface EmulationStationStoreState extends FetchStore, PostStore, FetchOptionsStore, ApiProviderStore {
  _baseUrl: string;
  _emulationstationOptions: EmulationStationConfigOptionsResponse;
  emulationstation: EmulationStationConfigResponse;
  currentState: EmulationStationCurrentState;
  themeRegion: string,
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
    currentState: {
      currentSystem: null,
      currentRom: null,
    },
    themeRegion: 'eu',
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
    async updateStatus(status: EventResponse) {
      const systemsStore = useSystemsStore();
      const { systems } = systemsStore;

      let { currentSystem } = this.currentState;
      let currentRom = null;

      if (status.System) {
        const storeSystem = systems.systems.filter((system) => system.name === status.System.SystemId)[0];
        if (this.currentState.currentSystem && storeSystem.name !== this.currentState.currentSystem.systemId) {
          this.resetCurrentSystem();
        }
        if (storeSystem) {
          currentSystem = {
            logoPath: `${api}/systems/${storeSystem.themeFolder}/resource/${this.themeRegion}/svg/logo`,
            consolePath: `${api}/systems/${storeSystem.themeFolder}/resource/${this.themeRegion}/svg/console`,
            gamePath: `${api}/systems/${storeSystem.themeFolder}/resource/${this.themeRegion}/svg/game`,
            name: status.System.System,
            systemId: status.System.SystemId,
            metaData: systemsMetaData[status.System.SystemId],
          };
          if (([Actions.runGame, Actions.runDemo].includes(status.Action)) && status.Game) {
            const encodedGamePath = encodeURIComponent(status.Game.GamePath);
            const metaData = await this.fetchRomMetaData(status.System.SystemId, encodedGamePath);
            currentRom = {
              name: status.Game.Game,
              imagePath: `${api}/systems/${storeSystem.name}/roms/metadata/image/${encodedGamePath}`,
              thumbnailPath: `${api}/systems/${storeSystem.name}/roms/metadata/thumbnail/${encodedGamePath}`,
              videoPath: `${api}/systems/${storeSystem.name}/roms/metadata/video/${encodedGamePath}`,
              developer: status.Game.Developer,
              publisher: status.Game.Publisher,
              players: status.Game.Players,
              region: status.Game.Region,
              genre: status.Game.Genre.replace(',', ', '),
              metaData,
            };
          }
        }
      }

      const currentState = {
        currentAction: status.Action,
        currentSystem,
        currentRom,
      };

      this.$patch({
        currentState,
      });
    },
    async fetchRomMetaData(systemName: string, encodedGamePath: string): Promise<RomMetaData> {
      let result = null;
      try {
        const response = await this._apiProvider.get(
          getPath(SYSTEMS.romsMetaData, { systemName, romPath: encodedGamePath }),
        );
        result = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
      return result;
    },
    async fetchStatus() {
      try {
        const response = await this._apiProvider.get(GLOBAL.status);
        const status = response.data;

        this.updateStatus(status);
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async getThemeRegion() {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        const response = await this._apiProvider.get(getPath(THEMES.region, { themeNameFolder: 'recalbox-next' }));
        this.themeRegion = response.data.region;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
