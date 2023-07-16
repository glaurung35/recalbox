/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { VERSIONS } from 'src/router/api.routes';
import { VersionsList } from 'stores/types/versionsList';

export type VersionsStoreState = {
  _baseUrl: string,
  versions: VersionsList,
};

export const useVersionsStore = defineStore('versions', {
  state: () => ({
    _baseUrl: VERSIONS,
    versions: {},
  } as VersionsStoreState),
});
