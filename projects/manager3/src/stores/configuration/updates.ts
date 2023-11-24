/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import {
  UpdatesConfigOptionsResponse,
  UpdatesConfigResponse,
} from 'stores/types/updatesConfig';

export type UpdatesStoreState = {
  _baseUrl: string,
  _updatesOptions: UpdatesConfigOptionsResponse,
  updates: UpdatesConfigResponse,
};

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
