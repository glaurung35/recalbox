/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import {
  ScraperConfigOptionsResponse,
  ScraperConfigResponse,
} from 'stores/types/scraper';

export type ScraperStoreState = {
  _baseUrl: string,
  _scraperOptions: ScraperConfigOptionsResponse,
  scraper: ScraperConfigResponse,
};

export const useScraperStore = defineStore('scraper', {
  state: () => ({
    _baseUrl: CONFIGURATION.scraper,
    _scraperOptions: {
      'screenscraper.media': {
        allowedStringList: [''],
      },
      getnamefrom: {
        lowerValue: 0,
        higherValue: 2,
      },
      'screenscraper.language': {
        allowedStringList: [''],
      },
      'screenscraper.region': {
        allowedStringList: [''],
      },
      'screenscraper.thumbnail': {
        allowedStringList: [''],
      },
      'screenscraper.video': {
        allowedStringList: [''],
      },
      source: {
        allowedStringList: [''],
      },
      'screenscraper.regionPriority': {
        allowedStringList: [''],
      },
    },
    scraper: {
      getnamefrom: {},
      extractregionfromfilename: {},
    },
  } as ScraperStoreState),

  getters: {
    mediaOptions: (state) => state._scraperOptions['screenscraper.media'].allowedStringList,
    languageOptions: (state) => state._scraperOptions['screenscraper.language'].allowedStringList.sort(),
    regionOptions: (state) => state._scraperOptions['screenscraper.region'].allowedStringList.sort(),
    thumbnailOptions: (state) => state._scraperOptions['screenscraper.thumbnail'].allowedStringList,
    videoOptions: (state) => state._scraperOptions['screenscraper.video'].allowedStringList,
    getNameFromOptions: () => [0, 1, 2],
    sourceOptions: (state) => state._scraperOptions.source.allowedStringList,
    regionPriorityOptions: (state) => state._scraperOptions['screenscraper.regionPriority'].allowedStringList,
  },
});
