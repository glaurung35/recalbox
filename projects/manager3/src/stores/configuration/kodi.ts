/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { formatStringList } from 'src/utils/formatStringList';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import { KodiConfigOptionsResponse, KodiConfigResponse } from 'stores/types/kodi';

export interface KodiStoreState extends FetchStore, PostStore, FetchOptionsStore {
  _baseUrl: string;
  _kodiOptions: KodiConfigOptionsResponse;
  kodi: KodiConfigResponse;
}

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
    kodi: {
      enabled: {
        value: true,
      },
      atstartup: {
        value: false,
      },
      xbutton: {
        value: false,
      },
      videomode: {
        value: '',
      },
    },
  } as KodiStoreState),

  getters: {
    videoModeOptions: (state) => formatStringList(state._kodiOptions.videomode),
    waitModeOptions: (state) => state._kodiOptions['network.waitmode'].allowedStringList.sort(),
  },
});
