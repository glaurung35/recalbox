/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { BIOS } from 'src/router/api.routes';
import { BiosList } from 'stores/types/biosList';

export type BiosStoreState = {
  _baseUrl: string,
  bios: BiosList,
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
          // eslint-disable-next-line @typescript-eslint/ban-ts-comment
          // @ts-ignore
          biosList.system = state.bios[systemName].fullName;
          list.push(biosList);
        });
      });

      return list;
    },
  },
});
