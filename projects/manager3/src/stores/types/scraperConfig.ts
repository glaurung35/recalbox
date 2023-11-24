/**
 * @author Nicolas TESSIER aka Asthonishia
 */

export interface ScraperConfigResponse {
  'screenscraper.password': {
    exist: boolean;
    value: string;
  };
  'screenscraper.media': {
    exist: boolean;
    value: string;
  };
  extractregionfromfilename: {
    exist: boolean;
    value: boolean;
  };
  getnamefrom: {
    exist: boolean;
    value: string;
  };
  'screenscraper.language': {
    exist: boolean;
    value: string;
  };
  'screenscraper.region': {
    exist: boolean;
    value: string;
  };
  'screenscraper.thumbnail': {
    exist: boolean;
    value: string;
  };
  'screenscraper.video': {
    exist: boolean;
    value: string;
  };
  'screenscraper.maps': {
    exist: boolean;
    value: boolean;
  };
  'screenscraper.manual': {
    exist: boolean;
    value: boolean;
  };
  'screenscraper.user': {
    exist: boolean;
    value: string;
  };
  source: {
    exist: boolean;
    value: string;
  };
  auto: {
    exist: boolean;
    value: boolean;
  };
  'screenscraper.p2k': {
    exist: boolean;
    value: boolean;
  };
  'screenscraper.regionPriority': {
    exist: boolean;
    value: string;
  };
}

export interface ScraperConfigOptionsResponse {
  'screenscraper.password': {
    type: string;
  };
  'screenscraper.media': {
    type: string;
    allowedStringList: string[];
  };
  extractregionfromfilename: {
    type: string;
  };
  getnamefrom: {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  'screenscraper.language': {
    type: string;
    allowedStringList: string[];
  };
  'screenscraper.region': {
    type: string;
    allowedStringList: string[];
  };
  'screenscraper.thumbnail': {
    type: string;
    allowedStringList: string[];
  };
  'screenscraper.video': {
    type: string;
    allowedStringList: string[];
  };
  'screenscraper.maps': {
    type: string;
  };
  'screenscraper.manual': {
    type: string;
  };
  'screenscraper.user': {
    type: string;
  };
  source: {
    type: string;
    allowedStringList: string[];
  };
  auto: {
    type: string;
  };
  'screenscraper.p2k': {
    type: string;
  };
  'screenscraper.regionPriority': {
    type: string;
    allowedStringList: string[];
  };
}
