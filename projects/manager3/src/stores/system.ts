import { defineStore } from 'pinia';
import { api } from 'boot/axios';
import { SYSTEM } from 'src/router/api.routes';

export const useSystemStore = defineStore('system', {
  state: () => ({
    _systemOptions: {
      language: {
        allowedStringList: [],
      },
      kblayout: {
        allowedStringList: [],
      },
      timezone: {
        allowedStringList: [],
      },
      'emulators.specialkeys': {
        allowedStringList: [],
      },
    },
    system: {},
  }),

  getters: {
    languageOptions: (state) => state._systemOptions.language.allowedStringList,
    kblayoutOptions: (state) => state._systemOptions.kblayout.allowedStringList,
    timezoneOptions: (state) => state._systemOptions.timezone.allowedStringList,
    specialkeysOptions: (state) => state._systemOptions['emulators.specialkeys'].allowedStringList,
  },

  actions: {
    async fetchOptions() {
      try {
        const response = await api.options(SYSTEM);
        this._systemOptions = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async fetch() {
      try {
        const response = await api.get(SYSTEM);
        this.system = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async post(data: Record<string, unknown>) {
      try {
        const response = await api.post(SYSTEM, data);
        this.system = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
