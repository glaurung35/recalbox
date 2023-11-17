/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { AudioConfig, AudioConfigOptions } from 'stores/types/audioConfig';
import { CONFIGURATION } from 'src/router/api.routes';

export type AudioStoreState = {
  _baseUrl: string,
  _audioOptions: AudioConfigOptions,
  audio: AudioConfig,
};

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
    deviceOptions: (state) => state._audioOptions.device.allowedStringList,
    volumeOptions: (state) => state._audioOptions.volume,
    musicVolumeOptions: (state) => state._audioOptions['music.volume'],
    modeOptions: (state) => state._audioOptions.mode.allowedStringList,
  },
});
