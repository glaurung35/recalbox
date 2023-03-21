import { defineStore } from 'pinia';
import { api } from 'boot/axios';
import { UPDATES } from 'src/router/api.routes';

export const useUpdatesStore = defineStore('updates', {
  state: () => ({
    _updatesOptions: {
      type: {
        allowedStringList: [],
      },
    },
    updates: {},
  }),

  getters: {
    updatesTypeOptions: (state) => state._updatesOptions.type.allowedStringList,
  },

  actions: {
    async fetchOptions() {
      try {
        const response = await api.options(UPDATES);
        this._updatesOptions = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async fetch() {
      try {
        const response = await api.get(UPDATES);
        this.updates = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async post(data: Record<string, unknown>) {
      try {
        const response = await api.post(UPDATES, data);
        this.updates = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
