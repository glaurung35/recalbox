/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { VERSIONS } from 'src/router/api.routes';
import { VersionsResponse } from 'stores/types/versions';

export type VersionsStoreState = {
  _baseUrl: string,
  versions: VersionsResponse,
};

export const useVersionsStore = defineStore('versions', {
  state: () => ({
    _baseUrl: VERSIONS,
    versions: {},
  } as VersionsStoreState),
});
