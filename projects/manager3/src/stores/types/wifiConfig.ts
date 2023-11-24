/**
 * @author Nicolas TESSIER aka Asthonishia
 */

export interface WifiConfigResponse {
  enabled: {
    exist: boolean;
    value: boolean;
  };
  netmask: {
    exist: boolean;
    value: string;
  };
  key: {
    exist: boolean;
    value: string;
  };
  ip: {
    exist: boolean;
    value: string;
  };
  ssid: {
    exist: boolean;
    value: string;
  };
  gateway: {
    exist: boolean;
    value: string;
  };
  region: {
    exist: boolean;
    value: string;
  };
  priority: {
    exist: boolean;
    value: number;
  };
  nameservers: {
    exist: boolean;
    value: string;
  };
}

export interface WifiConfigOptionsResponse {
  enabled: {
    type: string;
  };
  netmask: {
    type: string;
    allowedChars: string;
  };
  key: {
    type: string;
  };
  ip: {
    type: string;
    allowedChars: string;
  };
  ssid: {
    type: string;
  };
  gateway: {
    type: string;
    allowedChars: string;
  };
  region: {
    type: string;
    allowedStringList: string[];
  };
  priority: {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  nameservers: {
    type: string;
    allowedChars: string;
  };
}
