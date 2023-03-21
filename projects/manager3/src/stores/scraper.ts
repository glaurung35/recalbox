import { defineStore } from 'pinia';
import { api } from 'boot/axios';
import { SCRAPER } from 'src/router/api.routes';

export const useScraperStore = defineStore('scraper', {
  state: () => ({
    _scraperOptions: {
      'screenscraper.media': {
        allowedStringList: [],
      },
      'screenscraper.language': {
        allowedStringList: [],
      },
      'screenscraper.region': {
        allowedStringList: [],
      },
      'screenscraper.thumbnail': {
        allowedStringList: [],
      },
      'screenscraper.video': {
        allowedStringList: [],
      },
    },
    scraper: {
      getnamefrom: {},
      extractregionfromfilename: {},
    },
  }),

  getters: {
    mediaOptions: (state) => state._scraperOptions['screenscraper.media'].allowedStringList,
    languageOptions: (state) => state._scraperOptions['screenscraper.language'].allowedStringList,
    regionOptions: (state) => state._scraperOptions['screenscraper.region'].allowedStringList,
    thumbnailOptions: (state) => state._scraperOptions['screenscraper.thumbnail'].allowedStringList,
    videoOptions: (state) => state._scraperOptions['screenscraper.video'].allowedStringList,
    getNameFromOptions: () => [0, 1, 2],
  },

  actions: {
    async fetchOptions() {
      try {
        const response = await api.options(SCRAPER);
        this._scraperOptions = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async fetch() {
      try {
        const response = await api.get(SCRAPER);
        this.scraper = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
    async post(data: Record<string, unknown>) {
      try {
        const response = await api.post(SCRAPER, data);
        this.scraper = response.data;
      } catch (error) {
        // eslint-disable-next-line no-console
        console.log(error);
      }
    },
  },
});
