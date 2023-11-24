/**
 * @author Nicolas TESSIER aka Asthonishia
 */

export interface AudioConfigResponse {
  bgmusic: {
    exist: boolean;
    value: boolean;
  };
  volume: {
    exist: boolean;
    value: number;
  };
  'music.volume': {
    exist: boolean;
    value: number;
  };
  device: {
    exist: boolean;
    value: string;
  };
  mode: {
    exist: boolean;
    value: string;
  };
}

export interface AudioConfigOptionsResponse {
  bgmusic: {
    type: string;
  };
  volume: {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  'music.volume': {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  device: {
    type: string;
    allowedStringList: string[];
    displayableStringList: string[];
  };
  mode: {
    type: string;
    allowedStringList: string[];
  };
}
