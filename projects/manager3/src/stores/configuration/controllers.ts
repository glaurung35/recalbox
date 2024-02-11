/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import {
  ControllersConfigOptionsResponse, ControllersConfigResponse,
} from 'stores/types/controllers';

export interface ControllersStoreState extends FetchStore, PostStore, FetchOptionsStore {
  _baseUrl: string;
  _controllersOptions: ControllersConfigOptionsResponse;
  controllers: ControllersConfigResponse;
}

export const useControllersStore = defineStore('controllers', {
  state: () => ({
    _baseUrl: CONFIGURATION.controllers,
    _controllersOptions: {
      'ps3.driver': {
        allowedStringList: [''],
      },
    },
    controllers: {
      'bluetooth.enabled': {
        value: true,
      },
      'bluetooth.ertm': {
        value: true,
      },
      'ps3.enabled': {
        value: true,
      },
      'ps3.driver': {
        value: 'bluez',
      },
      'gpio.enabled': {
        value: false,
      },
      'gpio.args': {
        value: 'map=1,2',
      },
      'steam.enabled': {
        value: false,
      },
      'db9.enabled': {
        value: false,
      },
      'db9.args': {
        value: 'map=1',
      },
      'gamecon.enabled': {
        value: false,
      },
      'gamecon.args': {
        value: 'map=1',
      },
      'xarcade.enabled': {
        value: true,
      },
      'joycond.enabled': {
        value: true,
      },
      swapvalidateandcancel: {
        value: false,
      },
      'bluetooth.autopaironboot': {
        value: true,
      },
    },
  } as ControllersStoreState),

  getters: {
    ps3driverOptions: (state) => state._controllersOptions['ps3.driver'].allowedStringList.sort(),
  },
});
