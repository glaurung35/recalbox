/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { HYPERION } from 'src/router/api.routes';
import { HyperionConfig } from 'stores/types/hyperionConfig';

export type HyperionStoreState = {
  _baseUrl: string,
  hyperion: HyperionConfig,
}

export const useHyperionStore = defineStore('hyperion', {
  state: () => ({
    _baseUrl: HYPERION,
    hyperion: {},
  } as HyperionStoreState),
});
