import { defineStore } from 'pinia';
import { api } from 'boot/axios';
import { BIOS } from 'src/router/api.routes';

export const useBiosStore = defineStore('bios', {
  state: () => ({
    bios: {},
  }),

  getters: {
    biosList: (state) => {
      const list: object[] = [];

      Object.keys(state.bios).forEach((systemName) => {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        Object.keys(state.bios[systemName].biosList).forEach((biosName) => {
          // eslint-disable-next-line @typescript-eslint/ban-ts-comment
          // @ts-ignore
          const bios = state.bios[systemName].biosList[biosName];
          // eslint-disable-next-line @typescript-eslint/ban-ts-comment
          // @ts-ignore
          bios.system = state.bios[systemName].fullName;
          list.push(bios);
        });
      });

      return list;
    },
  },

  actions: {
    async fetch() {
      try {
        const response = await api.get(BIOS.all);
        this.bios = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
