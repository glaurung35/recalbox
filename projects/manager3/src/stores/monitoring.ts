/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { MONITORING } from 'src/router/api.routes';
import { toRaw } from 'vue';
import { StoragesResponse } from 'stores/types/storages';

export type MonitoringStoreState = {
  _baseUrl: string,
  monitoring: StoragesResponse,
};

export const useMonitoringStore = defineStore('monitoring', {
  state: () => ({
    _baseUrl: MONITORING.storageInfo,
    monitoring: {
      storages: {},
    },
    metrics: {
      temperatures: [],
      memory: [],
      cores: [],
    },
  } as MonitoringStoreState),

  actions: {
    getSharePercent() {
      if (Object.keys(this.monitoring.storages).length > 0) {
        const { storages } = this.monitoring;
        let result: {used: number, size: number} = { used: 0, size: 0 };

        Object.keys(storages).map((key): {used: number, size: number} => {
          if (storages[key as keyof typeof storages].recalbox === 'share') {
            result = toRaw(storages[key as keyof typeof storages]);
          }
          return { used: 0, size: 0 };
        });
        return Math.round((100 * result.used) / result.size);
      }
      return 0;
    },
  },
});
