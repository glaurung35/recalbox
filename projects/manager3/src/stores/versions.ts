/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { VERSIONS } from 'src/router/api.routes';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { VersionsResponse } from 'stores/types/versions';

export interface VersionsStoreState extends FetchStore {
  _baseUrl: string;
  versions: VersionsResponse;
}

export const useVersionsStore = defineStore('versions', {
  state: () => ({
    _baseUrl: VERSIONS,
    versions: {},
  } as VersionsStoreState),
});
