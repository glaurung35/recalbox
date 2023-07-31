/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { SYSTEM } from 'src/router/api.routes';

export type ServerStoreState = {
  available: boolean,
};

export const useServerStore = defineStore('server', {
  state: () => ({
    available: false,
  } as ServerStoreState),

  actions: {
    async reboot(): Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        await this._api80Provider.post(SYSTEM.reboot);
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async shutdown(): Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        await this._api80Provider.post(SYSTEM.shutdown);
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },

    async esStart(): Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        await this._api80Provider.post(SYSTEM.es.start);
        this.available = true;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async esStop(): Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        await this._api80Provider.post(SYSTEM.es.stop);
        this.available = false;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async esRestart(): Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        await this._api80Provider.post(SYSTEM.es.restart);
        this.available = true;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async supportArchive(): Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        const response = await this._api80Provider.get(SYSTEM.supportArchive);
        this.available = true;
      } catch (error) {
        console.log(error);
      }
    },
  },
});
