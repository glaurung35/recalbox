import { defineStore } from 'pinia';
import { VERSIONS } from 'src/router/api.routes';

export const useVersionsStore = defineStore('versions', {
  state: () => ({
    _baseUrl: VERSIONS,
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
});
