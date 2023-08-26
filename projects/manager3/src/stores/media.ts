/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { MEDIA } from 'src/router/api.routes';
import { date } from 'quasar';
import { MediasList } from 'stores/types/mediasList';
import { apiUrl } from 'boot/axios';

export type MediaStoreState = {
  _baseUrl: string,
  media: MediasList,
};

export const useMediaStore = defineStore('media', {
  state: () => ({
    _baseUrl: MEDIA.all,
    media: {
      mediaList: {},
    },
  } as MediaStoreState),

  getters: {
    screenshots: (state) => {
      const result: Array<object> = [];

      Object.keys(state.media.mediaList).forEach((key:string):void => {
        const extension:string = key.substring(key.length - 3).substring(0, key.length);
        // The screenshots directory have some text file, don't parse them
        if (extension === 'png') {
          if (key.includes('screenshot-')) {
            // Check screenshots created via current manager
            const name:string = key.substring(11).substring(0, 24);
            const formattedDate:string = date.formatDate(
              date.extractDate(name, 'YYYY-MM-DDTHH-mm-ss'), // "2023-04-03T07-51-41-443Z"
              'DD/MM/YYYY - HH:mm:ss',
            );
            result.push({
              name: key,
              // eslint-disable-next-line @typescript-eslint/ban-ts-comment
              // @ts-ignore
              url: apiUrl + MEDIA.get + key,
              date: formattedDate,
            });
          } else {
            // Check screenshots created in-game (HK + L1)
            const name:string = key.substring(key.length - 17).substring(0, key.length);
            const formattedDate:string = date.formatDate(
              date.extractDate(name, 'YYMMDD-HHmmss'),
              'DD/MM/YY - HH:mm-ss',
            );
            result.push({
              name: key,
              // eslint-disable-next-line @typescript-eslint/ban-ts-comment
              // @ts-ignore
              url: apiUrl + MEDIA.get + key,
              date: formattedDate,
            });
          }
        }
      });
      return result;
    },
  },

  actions: {
    async takeScreenshot():Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        const response = await this._apiProvider.post(MEDIA.takeScreenshot);
        this.media = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async delete(screenshotName: string):Promise<void> {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        const response = await this._apiProvider.delete(MEDIA.delete + screenshotName);
        this.media = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
