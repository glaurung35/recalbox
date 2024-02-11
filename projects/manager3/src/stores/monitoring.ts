/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { MONITORING } from 'src/router/api.routes';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { MonitoringResponse } from 'stores/types/mqtt';
import { toRaw } from 'vue';
import { StoragesResponse } from 'stores/types/storages';

export interface MonitoringStoreState extends FetchStore {
  _baseUrl: string;
  monitoring: StoragesResponse;
  metrics: {
    temperatures: [];
    memory: [];
    cores: [];
  };
}

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
    getFilteredStorages() {
      return Object.fromEntries(Object.entries(this.monitoring.storages).filter(
        (value, key) => this.monitoring.storages[key].recalbox !== 'unknown'
          && this.monitoring.storages[key].recalbox !== 'system',
      ));
    },
    getSharePercent() {
      if (Object.keys(this.monitoring.storages).length > 0) {
        const { storages } = this.monitoring;
        let result: {used: number, size: number} = { used: 0, size: 0 };

        Object.keys(storages).map((key): {used: number, size: number} => {
          if (storages[key].recalbox === 'share') {
            result = toRaw(storages[key]);
          }
          return { used: 0, size: 0 };
        });
        return Math.round((100 * result.used) / result.size);
      }
      return 0;
    },
    updateMonitoring(message: MonitoringResponse) {
      const temperatures = [
        ...this.metrics.temperatures,
        [
          (new Date(message.timestamp)).getTime(),
          // eslint-disable-next-line @typescript-eslint/ban-ts-comment
          // @ts-ignore
          parseFloat(message.temperature.temperatures.shift().toFixed(0)) ?? 0,
        ],
      ];

      const memory = [
        ...this.metrics.memory,
        [
          (new Date(message.timestamp)).getTime(),
          parseFloat(
            ((
              // eslint-disable-next-line @typescript-eslint/ban-ts-comment
              // @ts-ignore
              (message.memory.total - message.memory.available.shift()) / message.memory.total
            ) * 100).toFixed(0),
          ),
        ],
      ];

      const cores: { x: string, y: number }[] = Object.keys(message.cpus).map(
        (core): { x: string, y: number } => ({
          x: `Core ${core}`,
          // eslint-disable-next-line @typescript-eslint/ban-ts-comment
          // @ts-ignore
          y: parseFloat(message.cpus[core as keyof typeof message.cpus].consumption.shift().toFixed(0)),
        }),
      );

      this.$patch({
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        metrics: {
          cores,
          temperatures: temperatures.slice(-30),
          memory: memory.slice(-30),
        },
      });
    },
  },
});
