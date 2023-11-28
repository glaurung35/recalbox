/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { MEDIA } from 'src/router/api.routes';
import { date } from 'quasar';
import { MediasResponse } from 'stores/types/medias';
import { apiUrl } from 'boot/axios';

export type MediaStoreState = {
  _baseUrl: string,
  media: MediasResponse,
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

      Object.keys(state.media.mediaList).forEach((key): void => {
        // The screenshots directory have some text file, don't parse them
        if (key.includes('.png') || key.includes('.gif') || key.includes('.jpg') || key.includes('.svg')) {
          let formattedDate: string;
          if (key.includes('screenshot-')) {
            // Check screenshots created via current manager
            const name = key.substring(11).substring(0, 24);
            formattedDate = date.formatDate(
              date.extractDate(name, 'YYYY-MM-DDTHH-mm-ss'), // "2023-04-03T07-51-41-443Z"
              'DD/MM/YYYY - HH:mm:ss',
            );
          } else {
            // Check screenshots created in-game (HK + L1)
            const name = key.substring(key.length - 17).substring(0, key.length);
            formattedDate = date.formatDate(
              date.extractDate(name, 'YYMMDD-HHmmss'), // "230403-075141"
              'DD/MM/YYYY - HH:mm:ss',
            );
          }
          result.push({
            name: key,
            // eslint-disable-next-line @typescript-eslint/ban-ts-comment
            // @ts-ignore
            urlImage: apiUrl + MEDIA.get + key,
            date: formattedDate,
          });
        }

        // Do the videos
        if (key.includes('.mkv') || key.includes('.mp4') || key.includes('.avi')) {
          let formattedDate: string;
          const name = key.substring(key.length - 17).substring(0, key.length);
          // eslint-disable-next-line prefer-const
          formattedDate = date.formatDate(
            date.extractDate(name, 'YYMMDD-HHmmss'), // "230403-075141"
            'DD/MM/YYYY - HH:mm:ss',
          );
          result.push({
            name: key,
            // eslint-disable-next-line @typescript-eslint/ban-ts-comment
            // @ts-ignore
            urlVideo: apiUrl + MEDIA.get + key,
            date: formattedDate,
          });
        }
      });

      return result;
    },
  },

  actions: {
    async takeScreenshot() {
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
    async delete(screenshotName: string) {
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
