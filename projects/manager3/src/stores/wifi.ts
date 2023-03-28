import { defineStore } from 'pinia';
import { WIFI } from 'src/router/api.routes';
import { WifiConfig, WifiConfigOptions } from 'stores/types/wifiConfig';

export type WifiStoreState = {
  _baseUrl: string,
  _wifiOptions: WifiConfigOptions,
  wifi: WifiConfig,
};

export const useWifiStore = defineStore('wifi', {
  state: () => ({
    _baseUrl: WIFI,
    _wifiOptions: {
      region: {
        allowedStringList: [''],
      },
    },
    wifi: {},
  } as WifiStoreState),

  getters: {
    regionOptions: (state) => state._wifiOptions.region.allowedStringList,
  },
});
