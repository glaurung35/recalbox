/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { SYSTEMS } from 'src/router/api.routes';
import { toRaw } from 'vue';
import { SystemsList } from 'stores/types/systemsList';

export type SystemsStoreState = {
  _baseUrl: string,
  systems: SystemsList,
};

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
      const list: object[] = [];

      Object.keys(state.systems.systemList).forEach((system:string):void => {
        list.push(state.systems.systemList[system as keyof typeof state.systems.systemList]);
      });

      return list;
    },
  },

  actions: {
    getSystemsListCount():number {
      if (Object.keys(this.systems.systemList).length > 0) {
        return Object.keys(toRaw(this.systems.systemList)).length;
      }
      return 0;
    },
  },
});
