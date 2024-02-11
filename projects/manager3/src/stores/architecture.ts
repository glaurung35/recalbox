/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { GLOBAL } from 'src/router/api.routes';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { ArchitectureResponse } from 'stores/types/architecture';

export interface ArchitectureStoreState extends FetchStore {
  _baseUrl: string;
  architecture: ArchitectureResponse;
}

export const useArchitectureStore = defineStore('architecture', {
  state: () => ({
    _baseUrl: GLOBAL.architecture,
    architecture: {},
  } as ArchitectureStoreState),
});
