import { defineStore } from 'pinia';
import { MEDIA } from 'src/router/api.routes';
import { date } from 'quasar';

export const useMediaStore = defineStore('media', {
  state: () => ({
    _apiProvider: null,
    media: {
      mediaList: {},
    },
  }),

  getters: {
    screenshots: (state) => {
      const result: Array<object> = [];

      Object.keys(state.media.mediaList).forEach((key) => {
        const name:string = key;
        if (name.includes('screenshot-')) {
          result.push({
            name,
            url: MEDIA.get + name,
            date: date.formatDate(date.extractDate('2020-03-28', 'YYYY-MM-DD'), 'DD/MM/YYYY'),
          });
        }
      });
      return result;
    },
  },

  actions: {
    async fetch() {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        const response = await this._apiProvider.get(MEDIA.all);
        this.media = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async delete(name: string) {
      try {
        // eslint-disable-next-line @typescript-eslint/ban-ts-comment
        // @ts-ignore
        const response = await this._apiProvider.delete(MEDIA.delete + name);
        this.media = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
