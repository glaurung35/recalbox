/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { getPathBy, ROMS, SYSTEMS } from 'src/router/api.routes';
import { Rom } from 'src/stores/types/roms';

export interface RomsStoreState {
  roms: Rom[],
}

export const useRomsStore = defineStore('roms', {
  state: () => ({
    roms: [],
  } as RomsStoreState),

  actions: {
    async fetch() {
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
    async fetchBySystem(system: string) {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        const response = await this._apiProvider.get(getPathBy(SYSTEMS.roms, system));
        this.roms = response.data.roms;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
