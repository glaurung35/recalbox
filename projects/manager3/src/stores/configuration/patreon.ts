/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { PatreonConfig, PatreonConfigOptions } from 'stores/types/patreonConfig';

export type PatreonStoreState = {
  _baseUrl: string,
  _patreonOptions: PatreonConfigOptions,
  patreon: PatreonConfig,
};

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
