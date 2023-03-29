import { defineStore } from 'pinia';
import { AudioConfig, AudioConfigOptions } from 'stores/types/audioConfig';
import { AUDIO } from 'src/router/api.routes';

export type AudioStoreState = {
  _baseUrl: string,
  _audioOptions: AudioConfigOptions,
  audio: AudioConfig,
};

export const useAudioStore = defineStore('audio', {
  state: () => ({
    _baseUrl: AUDIO,
    _audioOptions: {
      device: {
        allowedStringList: [''],
      },
      volume: {},
    },
    audio: {
      volume: {},
    },
  } as AudioStoreState),

  getters: {
    deviceOptions: (state) => state._audioOptions.device.allowedStringList,
    volumeOptions: (state) => state._audioOptions.volume,
  },
});
