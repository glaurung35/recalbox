/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import { WifiBaseConfigOptionsResponse, WifiBaseConfigResponse } from 'stores/types/wifi';

export interface Wifi2StoreState extends FetchStore, PostStore, FetchOptionsStore {
  _baseUrl: string;
  _wifi2Options: WifiBaseConfigOptionsResponse;
  wifi2: WifiBaseConfigResponse;
}

export const useWifi2Store = defineStore('wifi2', {
  state: () => ({
    _baseUrl: CONFIGURATION.wifi2,
    _wifi2Options: {
    },
    wifi2: {},
  } as Wifi2StoreState),

  getters: {},
});
