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
        await this._apiProvider.post(SYSTEM.reboot);
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async shutdown(): Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        await this._apiProvider.post(SYSTEM.shutdown);
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },

    async esStart(): Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        await this._apiProvider.post(SYSTEM.es.start);
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async esStop(): Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        await this._apiProvider.post(SYSTEM.es.stop);
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async esRestart(): Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        await this._apiProvider.post(SYSTEM.es.restart);
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
