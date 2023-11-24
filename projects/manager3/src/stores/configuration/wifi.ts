/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { WifiConfigOptionsResponse, WifiConfigResponse } from 'stores/types/wifiConfig';

export type WifiStoreState = {
  _baseUrl: string,
  _wifiOptions: WifiConfigOptionsResponse,
  wifi: WifiConfigResponse,
};

export const useWifiStore = defineStore('wifi', {
  state: () => ({
    _baseUrl: CONFIGURATION.wifi,
    _wifiOptions: {
      region: {
        allowedStringList: [''],
      },
      priority: {
        lowerValue: 0,
        higherValue: 20,
      },
      netmask: {
        allowedChars: '',
      },
      ip: {
        allowedChars: '',
      },
      gateway: {
        allowedChars: '',
      },
      nameservers: {
        allowedChars: '',
      },
    },
    wifi: {
      priority: {},
    },
  } as WifiStoreState),

  getters: {
    regionOptions: (state) => state._wifiOptions.region.allowedStringList.sort(),
    priorityOptions: (state) => state._wifiOptions.priority,
  },
});
