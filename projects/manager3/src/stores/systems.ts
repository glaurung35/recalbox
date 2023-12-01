/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { SYSTEMS } from 'src/router/api.routes';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { toRaw } from 'vue';
import { System, SystemsResponse } from 'stores/types/systems';

export interface SystemsStoreState extends FetchStore {
  _baseUrl: string;
  systems: SystemsResponse;
}

export const useSystemsStore = defineStore('systems', {
  state: () => ({
    _baseUrl: SYSTEMS.all,
    systems: {
      romPath: '',
      systemList: {},
    },
  } as SystemsStoreState),

  getters: {
    systemsList: (state) => {
      const list: System[] = [];

      Object.keys(state.systems.systemList).forEach((system) => {
        list.push(state.systems.systemList[system as keyof typeof state.systems.systemList]);
      });

      return list;
    },
  },

  actions: {
    getSystemsListCount() {
      if (Object.keys(this.systems.systemList).length > 0) {
        return Object.keys(toRaw(this.systems.systemList)).length;
      }
      return 0;
    },
  },
});
