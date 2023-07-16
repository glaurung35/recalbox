/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { KodiConfig, KodiConfigOptions } from 'stores/types/kodiConfig';

export type KodiStoreState = {
  _baseUrl: string,
  _kodiOptions: KodiConfigOptions,
  kodi: KodiConfig,
};

export const useKodiStore = defineStore('kodi', {
  state: () => ({
    _baseUrl: CONFIGURATION.kodi,
    _kodiOptions: {},
    kodi: {},
  } as KodiStoreState),

  getters: {
    videoModeOptions: (state) => state._kodiOptions.videomode.allowedStringList,
    waitModeOptions: (state) => state._kodiOptions['network.waitmode'].allowedStringList,
  },
});
