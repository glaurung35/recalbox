/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import {
  ControllersConfigOptionsResponse, ControllersConfigResponse,
} from 'stores/types/controllers';

export interface ControllersStoreState extends FetchStore, PostStore, FetchOptionsStore {
  _baseUrl: string;
  _controllersOptions: ControllersConfigOptionsResponse;
  controllers: ControllersConfigResponse;
}

export const useControllersStore = defineStore('controllers', {
  state: () => ({
    _baseUrl: CONFIGURATION.controllers,
    _controllersOptions: {
      'ps3.driver': {
        allowedStringList: [''],
      },
    },
    controllers: {},
  } as ControllersStoreState),

  getters: {
    ps3driverOptions: (state) => state._controllersOptions['ps3.driver'].allowedStringList.sort(),
  },
});
