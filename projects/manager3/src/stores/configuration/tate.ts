/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { TateConfigOptionsResponse, TateConfigResponse } from 'stores/types/tate';

export type TateStoreState = {
  _baseUrl: string,
  _tateOptions: TateConfigOptionsResponse,
  tate: TateConfigResponse,
};

export const useTateStore = defineStore('tate', {
  state: () => ({
    _baseUrl: CONFIGURATION.tate,
    _tateOptions: {
      gamerotation: {
        allowedStringList: [''],
      },
    },
    tate: {},
  } as TateStoreState),

  getters: {
    gameRotationOptions: (state) => state._tateOptions.gamerotation.allowedStringList,
  },
});
