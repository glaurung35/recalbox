/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { UpdatesConfig, UpdatesConfigOptions } from 'stores/types/updatesConfig';

export type UpdatesStoreState = {
  _baseUrl: string,
  _updatesOptions: UpdatesConfigOptions,
  updates: UpdatesConfig,
};

export const useUpdatesStore = defineStore('updates', {
  state: () => ({
    _baseUrl: CONFIGURATION.updates,
    _updatesOptions: {
    },
    updates: {},
  } as UpdatesStoreState),

  getters: {},
});
