/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { WifiConfig, WifiConfigOptions } from 'stores/types/wifiConfig';

export type WifiStoreState = {
  _baseUrl: string,
  _wifiOptions: WifiConfigOptions,
  wifi: WifiConfig,
};

export const useWifiStore = defineStore('wifi', {
  state: () => ({
    _baseUrl: CONFIGURATION.wifi,
    _wifiOptions: {
      region: {
        allowedStringList: [''],
      },
      priority: {},
    },
    wifi: {
      priority: {},
    },
  } as WifiStoreState),

  getters: {
    regionOptions: (state) => state._wifiOptions.region.allowedStringList,
    priorityOptions: (state) => state._wifiOptions.priority,
  },
});
