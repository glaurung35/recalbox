/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { SYSTEMS } from 'src/router/api.routes';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { toRaw } from 'vue';
import { SystemsResponse } from 'stores/types/systems';

export interface SystemsStoreState extends FetchStore {
  _baseUrl: string;
  systems: SystemsResponse;
}

export const useSystemsStore = defineStore('systems', {
  state: () => ({
    _baseUrl: SYSTEMS.all,
    systems: {
      enumerations: {},
      systems: [],
    },
  } as SystemsStoreState),

  getters: {
    systemsList: (state) => state.systems.systems.filter(
      (system) => !system.name.includes('genre-') && !system.name.includes('arcade-'),
    ),
  },

  actions: {
    getSystemsListCount() {
      if (Object.keys(this.systems.systems).length > 0) {
        return Object.keys(toRaw(this.systems.systems)).length;
      }
      return 0;
    },
  },
});
