/**
 * @author Pit64
 */

export interface AutorunConfigResponse {
  enabled: {
    exist: boolean;
    value: boolean;
  };
  systemuuid: {
    exist: boolean;
    value: string;
  };
  gamepath: {
    exist: boolean;
    value: string;
  };
}

export interface AutorunConfigOptionsResponse {
  enabled: {
    type: string;
  };
  systemuuid: {
    type: string;
  };
  gamepath: {
    type: string;
  };
}
