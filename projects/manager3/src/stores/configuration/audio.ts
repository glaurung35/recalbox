/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { formatStringList } from 'src/utils/formatStringList';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import { AudioConfigOptionsResponse, AudioConfigResponse } from 'stores/types/audio';
import { CONFIGURATION } from 'src/router/api.routes';

export interface AudioStoreState extends FetchStore, PostStore, FetchOptionsStore {
  _baseUrl: string;
  _audioOptions: AudioConfigOptionsResponse;
  audio: AudioConfigResponse;
}

export const useAudioStore = defineStore('audio', {
  state: () => ({
    _baseUrl: CONFIGURATION.audio,
    _audioOptions: {
      device: {
        allowedStringList: [''],
        displayableStringList: [''],
      },
      volume: {
        lowerValue: 0,
        higherValue: 100,
      },
      'music.volume': {
        lowerValue: 0,
        higherValue: 100,
      },
      mode: {
        allowedStringList: [''],
      },
    },
    audio: {
      volume: {},
    },
  } as AudioStoreState),

  getters: {
    deviceOptions: (state) => formatStringList(state._audioOptions.device),
    volumeOptions: (state) => state._audioOptions.volume,
    musicVolumeOptions: (state) => state._audioOptions['music.volume'],
    modeOptions: (state) => state._audioOptions.mode.allowedStringList.sort(),
  },
});
