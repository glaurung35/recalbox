/**
 * @author Pit64
 */
import { defineStore } from 'pinia';
import { ARCHITECTURE } from 'src/router/api.routes';
import { ArchitectureResponse } from 'stores/types/architecture';

export type ArchitectureStoreState = {
  _baseUrl: string,
  architecture: ArchitectureResponse,
}

export const useArchitectureStore = defineStore('architecture', {
  state: () => ({
    _baseUrl: ARCHITECTURE,
    architecture: {},
  } as ArchitectureStoreState),
});
