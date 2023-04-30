/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { ControllersConfig, ControllersConfigOptions } from 'stores/types/controllersConfig';

export type ControllersStoreState = {
  _baseUrl: string,
  _controllersOptions: ControllersConfigOptions,
  controllers: ControllersConfig,
};

export const useControllersStore = defineStore('controllers', {
  state: () => ({
    _baseUrl: CONFIGURATION.controllers,
    _controllersOptions: {
      'ps3.driver': {
        allowedStringList: [''],
      },
    },
    controllers: {},
  } as ControllersStoreState),

  getters: {
    ps3driverOptions: (state) => state._controllersOptions['ps3.driver'].allowedStringList,
  },
});
