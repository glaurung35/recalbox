/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import {
  MusicConfigOptionsResponse,
  MusicConfigResponse,
} from 'stores/types/music';

export interface MusicStoreState extends FetchStore, PostStore {
  _baseUrl: string;
  _musicOptions: MusicConfigOptionsResponse;
  music: MusicConfigResponse;
}

export const useMusicStore = defineStore('music', {
  state: () => ({
    _baseUrl: CONFIGURATION.music,
    _musicOptions: {},
    music: {},
  } as MusicStoreState),

  getters: {},
});
