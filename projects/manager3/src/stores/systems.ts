/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { SYSTEMS } from 'src/router/api.routes';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { toRaw } from 'vue';
import { SystemsResponse, Type } from 'stores/types/systems';

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
    filteredSystemsList: (state) => state.systems.systems.filter(
      (system) => system.type !== Type.GameEngine
        && system.type !== Type.Port
        && system.type !== Type.VirtualSystem
        && system.type !== Type.VirtualArcade,
    ),
  },

  actions: {
    getSystemsListCount() {
      if (Object.keys(this.filteredSystemsList).length > 0) {
        return Object.keys(toRaw(this.filteredSystemsList)).length;
      }
      return 0;
    },
  },
});
