/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { PatronConfig, PatronConfigOptions } from 'stores/types/patronConfig';

export type PatronStoreState = {
  _baseUrl: string,
  _patronOptions: PatronConfigOptions,
  patron: PatronConfig,
};

export const usePatronStore = defineStore('patron', {
  state: () => ({
    _baseUrl: CONFIGURATION.patron,
    _patronOptions: {
      privatekey: {},
    },
    patron: {},
  } as PatronStoreState),

  getters: {},
});
