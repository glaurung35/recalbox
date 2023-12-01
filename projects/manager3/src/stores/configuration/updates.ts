/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import {
  UpdatesConfigOptionsResponse,
  UpdatesConfigResponse,
} from 'stores/types/updates';

export interface UpdatesStoreState extends FetchStore, PostStore, FetchOptionsStore {
  _baseUrl: string;
  _updatesOptions: UpdatesConfigOptionsResponse;
  updates: UpdatesConfigResponse;
}

export const useUpdatesStore = defineStore('updates', {
  state: () => ({
    _baseUrl: CONFIGURATION.updates,
    _updatesOptions: {
      type: {
        allowedChars: '',
      },
    },
    updates: {},
  } as UpdatesStoreState),

  getters: {},
});
