/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { ARCHITECTURE } from 'src/router/api.routes';
import { ArchitectureList } from 'stores/types/architectureList';

export type ArchitectureStoreState = {
  _baseUrl: string,
  architecture: ArchitectureList,
}

export const useArchitectureStore = defineStore('architecture', {
  state: () => ({
    _baseUrl: ARCHITECTURE,
    architecture: {},
  } as ArchitectureStoreState),
});
