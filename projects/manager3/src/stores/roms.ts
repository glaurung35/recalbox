/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { ROMS, SYSTEMS } from 'src/router/api.routes';

export const useRomsStore = defineStore('roms', {
  state: () => ({
    _apiProvider: null,
    roms: {},
  }),

  actions: {
    async fetch():Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        const response = await this._apiProvider.get(ROMS.all);
        this.roms = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async fetchBySystem(system:string):Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        const response = await this._apiProvider.get(`${SYSTEMS.root}/${system}/roms`);
        this.roms = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
