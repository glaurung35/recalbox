import { defineStore } from 'pinia';
import { SYSTEMS } from 'src/router/api.routes';
import { toRaw } from 'vue';

export const useSystemsStore = defineStore('systems', {
  state: () => ({
    _apiProvider: null,
    systems: {
      romPath: '',
      systemList: {},
    },
  }),

  getters: {
    systemsList: (state) => {
      const list: object[] = [];

      Object.keys(state.systems.systemList).forEach((system) => {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        list.push(state.systems.systemList[system]);
      });

      return list;
    },
  },

  actions: {
    async fetch() {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        const response = await this._apiProvider.get(SYSTEMS.all);
        this.systems = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    getSystemsListCount() {
      if (Object.keys(this.systems.systemList).length > 0) {
        return Object.keys(toRaw(this.systems.systemList)).length;
      }
      return 0;
    },
  },
});
