import { defineStore } from 'pinia';
import { api } from 'boot/axios';
import { VERSIONS } from 'src/router/api.routes';

export const useVersionsStore = defineStore('versions', {
  state: () => ({
    versions: {
      linux: {},
      recalbox: {},
      webapi: {},
      libretro: {
        retroarch: '',
        cores: {},
      },
    },
  }),

  actions: {
    async fetchVersions() {
      try {
        const response = await api.get(VERSIONS);
        this.versions = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
