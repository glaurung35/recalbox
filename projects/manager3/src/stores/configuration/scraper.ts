/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { formatStringList } from 'src/utils/formatStringList';
import { FetchOptionsStore } from 'stores/plugins/fetchOptionsStorePlugin';
import { FetchStore } from 'stores/plugins/fetchStorePlugin';
import { PostStore } from 'stores/plugins/postStorePlugin';
import {
  ScraperConfigOptionsResponse,
  ScraperConfigResponse,
} from 'stores/types/scraper';

export interface ScraperStoreState extends FetchStore, PostStore, FetchOptionsStore {
  _baseUrl: string;
  _scraperOptions: ScraperConfigOptionsResponse;
  scraper: ScraperConfigResponse;
}

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
        displayableStringList: [''],
      },
      'screenscraper.region': {
        allowedStringList: [''],
        displayableStringList: [''],
      },
      'screenscraper.thumbnail': {
        allowedStringList: [''],
      },
      'screenscraper.video': {
        allowedStringList: [''],
      },
      source: {
        allowedStringList: [''],
        displayableStringList: [''],
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
    mediaOptions: (state) => state._scraperOptions['screenscraper.media'].allowedStringList.sort(),
    languageOptions: (state) => formatStringList(state._scraperOptions['screenscraper.language']),
    regionOptions: (state) => formatStringList(state._scraperOptions['screenscraper.region']),
    thumbnailOptions: (state) => state._scraperOptions['screenscraper.thumbnail'].allowedStringList.sort(),
    videoOptions: (state) => state._scraperOptions['screenscraper.video'].allowedStringList.sort(),
    getNameFromOptions: () => [0, 1, 2],
    sourceOptions: (state) => formatStringList(state._scraperOptions.source),
    regionPriorityOptions: (state) => state._scraperOptions['screenscraper.regionPriority'].allowedStringList.sort(),
  },
});
