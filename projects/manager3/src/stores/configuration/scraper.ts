/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { defineStore } from 'pinia';
import { CONFIGURATION } from 'src/router/api.routes';
import { ScraperConfig, ScraperConfigOptions } from 'stores/types/scraperConfig';

export type ScraperStoreState = {
  _baseUrl: string,
  _scraperOptions: ScraperConfigOptions,
  scraper: ScraperConfig,
};

export const useScraperStore = defineStore('scraper', {
  state: () => ({
    _baseUrl: CONFIGURATION.scraper,
    _scraperOptions: {
      'screenscraper.password': {
        type: '',
      },
      'screenscraper.media': {
        type: '',
        allowedStringList: [''],
      },
      extractregionfromfilename: {
        type: '',
      },
      getnamefrom: {
        type: '',
        lowerValue: 0,
        higherValue: 2,
      },
      'screenscraper.language': {
        type: '',
        allowedStringList: [''],
      },
      'screenscraper.region': {
        type: '',
        allowedStringList: [''],
      },
      'screenscraper.thumbnail': {
        type: '',
        allowedStringList: [''],
      },
      'screenscraper.video': {
        type: '',
        allowedStringList: [''],
      },
      'screenscraper.maps': {
        type: '',
      },
      'screenscraper.manual': {
        type: '',
      },
      'screenscraper.user': {
        type: '',
      },
      source: {
        type: '',
        allowedStringList: [''],
      },
      'screenscraper.regionPriority': {
        type: '',
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
    languageOptions: (state) => state._scraperOptions['screenscraper.language'].allowedStringList,
    regionOptions: (state) => state._scraperOptions['screenscraper.region'].allowedStringList,
    thumbnailOptions: (state) => state._scraperOptions['screenscraper.thumbnail'].allowedStringList,
    videoOptions: (state) => state._scraperOptions['screenscraper.video'].allowedStringList,
    getNameFromOptions: () => [0, 1, 2],
    sourceOptions: (state) => state._scraperOptions.source.allowedStringList,
    regionPriorityOptions: (state) => state._scraperOptions['screenscraper.regionPriority'].allowedStringList,
  },
});
