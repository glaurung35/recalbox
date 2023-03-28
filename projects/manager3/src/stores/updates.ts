import { defineStore } from 'pinia';
import { UPDATES } from 'src/router/api.routes';
import { UpdatesConfig, UpdatesConfigOptions } from 'stores/dtos/updatesConfig';

export type UpdatesStoreState = {
  _baseUrl: string,
  _updatesOptions: UpdatesConfigOptions,
  updates: UpdatesConfig,
};

export const useUpdatesStore = defineStore('updates', {
  state: () => ({
    _baseUrl: UPDATES,
    _updatesOptions: {
      type: {
        allowedStringList: [''],
      },
    },
    updates: {},
  } as UpdatesStoreState),

  getters: {
    updatesTypeOptions: (state) => state._updatesOptions.type.allowedStringList,
  },
});
