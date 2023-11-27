/**
 * @author Nicolas TESSIER aka Asthonishia
 */

export interface UpdatesConfigResponse {
  enabled: {
    exist: boolean;
    value: boolean;
  };
  type: {
    exist: boolean;
    value: string;
  };
}

export interface UpdatesConfigOptionsResponse {
  enabled: {
    type: string;
  };
  type: {
    type: string;
    allowedChars: string;
  };
}
