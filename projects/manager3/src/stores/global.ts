import { defineStore } from 'pinia';
import { api } from 'boot/axios';
import { GLOBAL } from 'src/router/api.routes';

export const useGlobalStore = defineStore('global', {
  state: () => ({
    _globalOptions: {
      ratio: {
        allowedStringList: [],
      },
      shaderset: {
        allowedStringList: [],
      },
      'shaderset.file': {
        allowedStringList: [],
      },
    },
    global: {},
  }),

  getters: {
    ratioOptions: (state) => state._globalOptions.ratio.allowedStringList,
    shadersetOptions: (state) => state._globalOptions.shaderset.allowedStringList,
    shadersetfileOptions: (state) => state._globalOptions['shaderset.file'].allowedStringList,
  },

  actions: {
    async fetchOptions() {
      try {
        const response = await api.options(GLOBAL);
        this._globalOptions = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async fetch() {
      try {
        const response = await api.get(GLOBAL);
        this.global = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async post(data: Record<string, unknown>) {
      try {
        const response = await api.post(GLOBAL, data);
        this.global = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
