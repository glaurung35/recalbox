import { defineStore } from 'pinia';
import { api } from 'boot/axios';
import { AUDIO } from 'src/router/api.routes';

export const useAudioStore = defineStore('audio', {
  state: () => ({
    _audioOptions: {
      device: {
        allowedStringList: [],
      },
    },
    audio: {
      bgmusic: {},
      volume: {},
      device: {},
    },
  }),

  getters: {
    deviceOptions: (state) => state._audioOptions.device.allowedStringList,
  },

  actions: {
    async fetchOptions() {
      try {
        const response = await api.options(AUDIO);
        this._audioOptions = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async fetch() {
      try {
        const response = await api.get(AUDIO);
        this.audio = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async post(data: Record<string, unknown>) {
      try {
        const response = await api.post(AUDIO, data);
        this.audio = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
