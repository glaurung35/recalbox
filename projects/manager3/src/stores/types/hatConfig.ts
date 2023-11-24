/**
 * @author Pit64
 */

export interface HatConfigResponse {
  'wpaf.enabled': {
    exist: boolean;
    value: boolean;
  };
  'wpaf.board': {
    exist: boolean;
    value: string;
  };
}

export interface HatConfigOptionsResponse {
  'wpaf.enabled': {
    type: string;
  };
  'wpaf.board': {
    type: string;
    allowedStringList: string[];
  };
}
