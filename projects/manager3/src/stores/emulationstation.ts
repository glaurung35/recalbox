import { defineStore } from 'pinia';
import { api } from 'boot/axios';
import { EMUSTATION } from 'src/router/api.routes';

export const useEmulationstationStore = defineStore('emulationstation', {
  state: () => ({
    _emulationstationOptions: {
      menu: {
        allowedStringList: [],
      },
      selectedsystem: {
        allowedStringList: [],
      },
    },
    emulationstation: {
      bootongamelist: {},
      hidesystemview: {},
      gamelistonly: {},
    },
  }),

  getters: {
    menuOptions: (state) => state._emulationstationOptions.menu.allowedStringList,
    selectedsystemOptions: (state) => state._emulationstationOptions
      .selectedsystem.allowedStringList,
  },

  actions: {
    async fetchOptions() {
      try {
        const response = await api.options(EMUSTATION);
        this._emulationstationOptions = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async fetch() {
      try {
        const response = await api.get(EMUSTATION);
        this.emulationstation = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async post(data: Record<string, unknown>) {
      try {
        const response = await api.post(EMUSTATION, data);
        this.emulationstation = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
