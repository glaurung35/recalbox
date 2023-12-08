/**
 * @author Pit64
 */
export interface TateConfigResponse {
  gamerotation: {
    exist: boolean;
    value: string;
  };
}

export interface TateConfigOptionsResponse {
  gamerotation: {
    type: string;
    allowedStringList: string[];
  };
}
