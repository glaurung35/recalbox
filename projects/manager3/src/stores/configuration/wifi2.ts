/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { Wifi2Config, Wifi2ConfigOptions } from 'stores/types/wifi2Config';

export type Wifi2StoreState = {
  _baseUrl: string,
  _wifi2Options: Wifi2ConfigOptions,
  wifi2: Wifi2Config,
};

export const useWifi2Store = defineStore('wifi2', {
  state: () => ({
    _baseUrl: CONFIGURATION.wifi2,
    _wifi2Options: {
    },
    wifi2: {},
  } as Wifi2StoreState),

  getters: {},
});
