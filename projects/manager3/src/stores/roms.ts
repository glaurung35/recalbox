/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { ROMS, SYSTEMS } from 'src/router/api.routes';
import { ApiProviderStore } from 'stores/plugins/apiProviderStorePlugin';

export interface RomsStoreState extends ApiProviderStore {
  roms: object;
}

export const useRomsStore = defineStore('roms', {
  state: () => ({
    roms: {},
  } as RomsStoreState),

  actions: {
    async fetch() {
      try {
        const response = await this._apiProvider.get(ROMS.all);
        this.roms = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async fetchBySystem(system: string) {
      try {
        const response = await this._apiProvider.get(`${SYSTEMS.root}/${system}/roms`);
        this.roms = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
