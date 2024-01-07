/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { SYSTEM } from 'src/router/api.routes';
import { Api80ProviderStore } from 'stores/plugins/api80ProviderStorePlugin';
import { ServerResponse } from 'stores/types/server';

export interface ServerStoreState extends Api80ProviderStore {
  available: boolean;
  server: ServerResponse;
}

export const useServerStore = defineStore('server', {
  state: () => ({
    available: false,
    server: {
      supportArchive: {},
    },
  } as ServerStoreState),

  actions: {
    async reboot(): Promise<void> {
      try {
        await this._api80Provider.post(SYSTEM.reboot);
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async shutdown(): Promise<void> {
      try {
        await this._api80Provider.post(SYSTEM.shutdown);
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },

    async esStart(): Promise<void> {
      try {
        await this._api80Provider.post(SYSTEM.es.start);
        this.available = true;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async esShutdown(): Promise<void> {
      try {
        await this._api80Provider.post(SYSTEM.es.stop);
        this.available = false;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async esReboot(): Promise<void> {
      try {
        await this._api80Provider.post(SYSTEM.es.restart);
        this.available = true;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async supportArchive(): Promise<void> {
      try {
        const response = await this._api80Provider.get(SYSTEM.supportArchive);
        this.server = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
