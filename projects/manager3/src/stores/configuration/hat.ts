/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import {
  HatConfigOptionsResponse,
  HatConfigResponse,
} from 'stores/types/hat';

export interface HatStoreState extends FetchStore, PostStore, FetchOptionsStore {
  _baseUrl: string;
  _hatOptions: HatConfigOptionsResponse;
  hat: HatConfigResponse;
}

export const useHatStore = defineStore('hat', {
  state: () => ({
    _baseUrl: CONFIGURATION.hat,
    _hatOptions: {
      'wpaf.board': {
        allowedStringList: [''],
      },
    },
    hat: {},
  } as HatStoreState),

  getters: {
    boardOptions: (state) => state._hatOptions['wpaf.board'].allowedStringList.sort(),
  },
});
