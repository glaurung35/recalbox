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
      (system) => Type[system.type as keyof typeof Type] !== Type[6]
        && Type[system.type as keyof typeof Type] !== Type[7]
        && Type[system.type as keyof typeof Type] !== Type[8]
        && Type[system.type as keyof typeof Type] !== Type[9],
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
