import { defineStore } from 'pinia';
import { api } from 'boot/axios';
import { ROMS, SYSTEMS } from 'src/router/api.routes';

export const useRomsStore = defineStore('roms', {
  state: () => ({
    roms: {},
  }),

  actions: {
    async fetch() {
      try {
        const response = await api.get(ROMS.all);
        this.roms = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async fetchBySystem(system:string) {
      try {
        const response = await api.get(`${SYSTEMS.root}/${system}/roms`);
        this.roms = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
