/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { HatConfig, HatConfigOptions } from 'stores/types/hatConfig';

export type HatStoreState = {
  _baseUrl: string,
  _hatOptions: HatConfigOptions,
  hat: HatConfig,
};

export const useHatStore = defineStore('hat', {
  state: () => ({
    _baseUrl: CONFIGURATION.hat,
    _hatOptions: {},
    hat: {},
  } as HatStoreState),

  getters: {
    boardOptions: (state) => state._hatOptions['wpaf.board'].allowedStringList,
  },
});
