/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import {
  MusicConfigOptionsResponse,
  MusicConfigResponse,
} from 'stores/types/musicConfig';

export type MusicStoreState = {
  _baseUrl: string,
  _musicOptions: MusicConfigOptionsResponse,
  music: MusicConfigResponse,
};

export const useMusicStore = defineStore('music', {
  state: () => ({
    _baseUrl: CONFIGURATION.music,
    _musicOptions: {},
    music: {},
  } as MusicStoreState),

  getters: {},
});
