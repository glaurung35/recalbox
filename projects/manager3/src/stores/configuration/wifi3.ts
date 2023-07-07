/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { Wifi3Config, Wifi3ConfigOptions } from 'stores/types/wifi3Config';

export type Wifi3StoreState = {
  _baseUrl: string,
  _wifi3Options: Wifi3ConfigOptions,
  wifi3: Wifi3Config,
};

export const useWifi3Store = defineStore('wifi3', {
  state: () => ({
    _baseUrl: CONFIGURATION.wifi3,
    _wifi3Options: {
    },
    wifi3: {},
  } as Wifi3StoreState),

  getters: {},
});
