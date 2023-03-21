import { defineStore } from 'pinia';
import { api } from 'boot/axios';
import { HYPERION } from 'src/router/api.routes';

export const useHyperionStore = defineStore('hyperion', {
  state: () => ({
    hyperion: {
      enabled: {},
    },
  }),

  actions: {
    async fetch() {
      try {
        const response = await api.get(HYPERION);
        this.hyperion = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async post(data: Record<string, unknown>) {
      try {
        const response = await api.post(HYPERION, data);
        this.hyperion = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
