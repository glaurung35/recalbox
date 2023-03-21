import { defineStore } from 'pinia';
import { api } from 'boot/axios';
import { KODI } from 'src/router/api.routes';

export const useKodiStore = defineStore('kodi', {
  state: () => ({
    _kodiOptions: {
      videomode: {
        allowedStringList: [],
      },
      'network.waitmode': {
        allowedStringList: [],
      },
    },
    kodi: {
      enabled: {},
      atstartup: {},
      xbutton: {},
      videomode: {},
    },
  }),

  getters: {
    videoModeOptions: (state) => state._kodiOptions.videomode.allowedStringList,
    waitModeOptions: (state) => state._kodiOptions['network.waitmode'].allowedStringList,
  },

  actions: {
    async fetchOptions() {
      try {
        const response = await api.options(KODI);
        this._kodiOptions = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async fetch() {
      try {
        const response = await api.get(KODI);
        this.kodi = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async post(data: Record<string, unknown>) {
      try {
        const response = await api.post(KODI, data);
        this.kodi = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
