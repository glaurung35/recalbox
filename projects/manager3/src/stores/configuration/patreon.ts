/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import {
  PatreonConfigOptionsResponse,
  PatreonConfigResponse,
} from 'stores/types/patreon';

export interface PatreonStoreState extends FetchStore, PostStore {
  _baseUrl: string;
  _patreonOptions: PatreonConfigOptionsResponse;
  patreon: PatreonConfigResponse;
}

export const usePatreonStore = defineStore('patreon', {
  state: () => ({
    _baseUrl: CONFIGURATION.patreon,
    _patreonOptions: {
      privatekey: {
        allowedChars: '',
      },
    },
    patreon: {},
  } as PatreonStoreState),

  getters: {},
});
