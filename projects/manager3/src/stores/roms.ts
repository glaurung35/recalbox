/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { getPath, ROMS, SYSTEMS } from 'src/router/api.routes';
import { Rom } from 'src/stores/types/roms';
import { ApiProviderStore } from 'stores/plugins/apiProviderStorePlugin';

export interface RomsStoreState extends ApiProviderStore {
  roms: Rom[]|[],
  total: number,
}

export const useRomsStore = defineStore('roms', {
  state: () => ({
    roms: [],
    total: 0,
  } as RomsStoreState),

  actions: {
    async getRomsCount() {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        const response = await this._apiProvider.get(ROMS.total);
        this.total = response.data.total;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async fetchBySystem(system: string) {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        const response = await this._apiProvider.get(getPath(SYSTEMS.roms, { systemName: system }));
        this.roms = response.data.roms;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
