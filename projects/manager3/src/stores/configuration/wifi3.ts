/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import { WifiBaseConfigOptionsResponse, WifiBaseConfigResponse } from 'stores/types/wifi';

export interface Wifi3StoreState extends FetchStore, PostStore, FetchOptionsStore {
  _baseUrl: string;
  _wifi3Options: WifiBaseConfigOptionsResponse;
  wifi3: WifiBaseConfigResponse;
}

export const useWifi3Store = defineStore('wifi3', {
  state: () => ({
    _baseUrl: CONFIGURATION.wifi3,
    _wifi3Options: {
    },
    wifi3: {},
  } as Wifi3StoreState),

  getters: {},
});
