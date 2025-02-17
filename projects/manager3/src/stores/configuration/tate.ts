/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import { TateConfigOptionsResponse, TateConfigResponse } from 'stores/types/tate';

export interface TateStoreState extends FetchStore, PostStore, FetchOptionsStore {
  _baseUrl: string;
  _tateOptions: TateConfigOptionsResponse;
  tate: TateConfigResponse;
}

export const useTateStore = defineStore('tate', {
  state: () => ({
    _baseUrl: CONFIGURATION.tate,
    _tateOptions: {
      gamerotation: {
        allowedStringList: [''],
      },
    },
    tate: {
      gamerotation: {
        value: '0',
      },
    },
  } as TateStoreState),

  getters: {
    gameRotationOptions: (state) => state._tateOptions.gamerotation.allowedStringList.sort(),
  },
});
