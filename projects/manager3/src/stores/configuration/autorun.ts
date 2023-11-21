/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { AutorunConfig, AutorunConfigOptions } from 'stores/types/autorunConfig';

export type AutorunStoreState = {
  _baseUrl: string,
  _autorunOptions: AutorunConfigOptions,
  autorun: AutorunConfig,
};

export const useAutorunStore = defineStore('autorun', {
  state: () => ({
    _baseUrl: CONFIGURATION.autorun,
    _autorunOptions: {},
    autorun: {},
  } as AutorunStoreState),

  getters: {},
});
