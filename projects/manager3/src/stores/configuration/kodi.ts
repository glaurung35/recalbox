/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { KodiConfigOptionsResponse, KodiConfigResponse } from 'stores/types/kodiConfig';

export type KodiStoreState = {
  _baseUrl: string,
  _kodiOptions: KodiConfigOptionsResponse,
  kodi: KodiConfigResponse,
};

export const useKodiStore = defineStore('kodi', {
  state: () => ({
    _baseUrl: CONFIGURATION.kodi,
    _kodiOptions: {
      'network.waithost': {
        allowedChars: '',
      },
      'network.waitmode': {
        allowedStringList: [''],
      },
      'network.waittime': {
        lowerValue: 0,
        higherValue: 2147483647,
      },
      videomode: {
        allowedStringList: [''],
        displayableStringList: [''],
      },
    },
    kodi: {},
  } as KodiStoreState),

  getters: {
    videoModeOptions: (state) => state._kodiOptions.videomode.allowedStringList,
    waitModeOptions: (state) => state._kodiOptions['network.waitmode'].allowedStringList,
  },
});
