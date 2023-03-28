import { defineStore } from 'pinia';
import { MONITORING } from 'src/router/api.routes';
import { toRaw } from 'vue';

export const useMonitoringStore = defineStore('monitoring', {
  state: () => ({
    _apiProvider: null,
    storageInfo: {
      storages: {},
    },
    metrics: {
      temperatures: [],
      memory: [],
      cores: [],
    },
  }),

  actions: {
    async fetch() {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        const response = await this._apiProvider.get(MONITORING.storageInfo);
        this.storageInfo = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },

    getSharePercent():number {
      if (Object.keys(this.storageInfo.storages).length > 0) {
        const { storages } = this.storageInfo;
        let result = { used: 0, size: 0 };

        Object.keys(storages).map((key) => {
          // eslint-disable-next-line @typescript-eslint/ban-ts-comment
          // @ts-ignore
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
