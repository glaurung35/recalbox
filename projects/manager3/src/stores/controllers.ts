import { defineStore } from 'pinia';
import { api } from 'boot/axios';
import { CONTROLLERS } from 'src/router/api.routes';

export const useControllersStore = defineStore('controllers', {
  state: () => ({
    _controllersOptions: {
      'ps3.driver': {
        allowedStringList: [],
      },
    },
    controllers: {},
  }),

  getters: {
    ps3driverOptions: (state) => state._controllersOptions['ps3.driver'].allowedStringList,
  },

  actions: {
    async fetchOptions() {
      try {
        const response = await api.options(CONTROLLERS);
        this._controllersOptions = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async fetch() {
      try {
        const response = await api.get(CONTROLLERS);
        this.controllers = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async post(data: Record<string, unknown>) {
      try {
        const response = await api.post(CONTROLLERS, data);
        this.controllers = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
