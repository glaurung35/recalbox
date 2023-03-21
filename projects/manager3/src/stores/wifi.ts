import { defineStore } from 'pinia';
import { api } from 'boot/axios';
import { WIFI } from 'src/router/api.routes';

export const useWifiStore = defineStore('wifi', {
  state: () => ({
    _wifiOptions: {
      device: {
        allowedStringList: [],
      },
    },
    wifi: {},
  }),

  actions: {
    async fetchOptions() {
      try {
        const response = await api.options(WIFI);
        this._wifiOptions = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async fetch() {
      try {
        const response = await api.get(WIFI);
        this.wifi = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async post(data: Record<string, unknown>) {
      try {
        const response = await api.post(WIFI, data);
        this.wifi = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
