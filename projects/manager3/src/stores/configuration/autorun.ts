/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import {
  AutorunConfigOptionsResponse,
  AutorunConfigResponse,
} from 'stores/types/autorun';

export interface AutorunStoreState extends FetchStore, PostStore, FetchOptionsStore {
  _baseUrl: string;
  _autorunOptions: AutorunConfigOptionsResponse;
  autorun: AutorunConfigResponse;
}

export const useAutorunStore = defineStore('autorun', {
  state: () => ({
    _baseUrl: CONFIGURATION.autorun,
    _autorunOptions: {},
    autorun: {},
  } as AutorunStoreState),

  getters: {},
});
