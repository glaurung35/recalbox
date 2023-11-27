/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { BIOS } from 'src/router/api.routes';
import { BiosResponse } from 'stores/types/bios';

export type BiosStoreState = {
  _baseUrl: string,
  bios: BiosResponse,
};

export const useBiosStore = defineStore('bios', {
  state: () => ({
    _baseUrl: BIOS.all,
    bios: {},
  } as BiosStoreState),

  getters: {
    biosList: (state) => {
      const list: object[] = [];

      Object.keys(state.bios).forEach((systemName) => {
        Object.keys(state.bios[systemName].biosList).forEach((biosName) => {
          const biosList = state.bios[systemName].biosList[biosName];
          biosList.system = state.bios[systemName].fullName;
          list.push(biosList);
        });
      });

      return list;
    },
  },
});
