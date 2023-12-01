/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import { HyperionConfigResponse } from 'stores/types/hyperion';

export interface HyperionStoreState extends FetchStore, PostStore {
  _baseUrl: string;
  hyperion: HyperionConfigResponse;
}

export const useHyperionStore = defineStore('hyperion', {
  state: () => ({
    _baseUrl: CONFIGURATION.hyperion,
    hyperion: {},
  } as HyperionStoreState),
});
