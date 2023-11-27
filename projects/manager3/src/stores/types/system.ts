/**
 * @author Nicolas TESSIER aka Asthonishia
 */

export interface SystemConfigResponse {
  'emulators.specialkeys': {
    exist: boolean;
    value: string;
  };
  'splash.length': {
    exist: boolean;
    value: string;
  };
  'splash.select': {
    exist: boolean;
    value: string;
  };
  hostname: {
    exist: boolean;
    value: string;
  };
  'virtual-gamepads.enabled': {
    exist: boolean;
    value: boolean;
  };
  'ssh.enabled': {
    exist: boolean;
    value: boolean;
  };
  kblayout: {
    exist: boolean;
    value: string;
  };
  language: {
    exist: boolean;
    value: string;
  };
  'samba.enabled': {
    exist: boolean;
    value: boolean;
  };
  timezone: {
    exist: boolean;
    value: string;
  };
  'manager.enabled': {
    exist: boolean;
    value: boolean;
  };
  'es.videomode': {
    exist: boolean;
    value: string;
  };
  'power.switch': {
    exist: boolean;
    value: string;
  };
  'fbcp.enabled': {
    exist: boolean;
    value: boolean;
  };
  'api.enabled': {
    exist: boolean;
    value: boolean;
  };
  overscan: {
    exist: boolean;
    value: boolean;
  };
  'secondminitft.enabled': {
    exist: boolean;
    value: boolean;
  };
  'secondminitft.type': {
    exist: boolean;
    value: string;
  };
  'secondminitft.resolution': {
    exist: boolean;
    value: string;
  };
  'secondminitft.imagestretchenabled': {
    exist: boolean;
    value: boolean;
  };
  'secondminitft.imageenlargeenabled': {
    exist: boolean;
    value: boolean;
  };
  'secondminitft.imagealphaenabled': {
    exist: boolean;
    value: boolean;
  };
  'secondminitft.imageignoreaspectenabled': {
    exist: boolean;
    value: boolean;
  };
  'secondminitft.disablevideoines': {
    exist: boolean;
    value: boolean;
  };
  'secondminitft.backlightcontrol': {
    exist: boolean;
    value: number;
  };
  'secondminitft.usemarquee': {
    exist: boolean;
    value: boolean;
  };
  'secondminitft.sleepenabled': {
    exist: boolean;
    value: boolean;
  };
  'externalscreen.prefered': {
    exist: boolean;
    value: string;
  };
  'externalscreen.forceresolution': {
    exist: boolean;
    value: string;
  };
  'externalscreen.forcefrequency': {
    exist: boolean;
    value: string;
  };
}

export interface SystemConfigOptionsResponse {
  'emulators.specialkeys': {
    type: string;
    allowedStringList: string[];
  };
  'splash.length': {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  'splash.select': {
    type: string;
    allowedStringList: string[];
  };
  hostname: {
    type: string;
    allowedChars: string;
  };
  'virtual-gamepads.enabled': {
    type: string;
  };
  'ssh.enabled': {
    type: string;
  };
  kblayout: {
    type: string;
    allowedStringList: string[];
    displayableStringList: string[];
  };
  language: {
    type: string;
    allowedStringList: string[];
    displayableStringList: string[];
  };
  'samba.enabled': {
    type: string;
  };
  timezone: {
    type: string;
    allowedStringList: string[];
  };
  'manager.enabled': {
    type: string;
  };
  'es.videomode': {
    type: string;
    allowedStringList: string[];
    displayableStringList: string[];
  };
  'power.switch': {
    type: string;
    allowedStringList: string[];
  };
  'fbcp.enabled': {
    type: string;
  };
  'api.enabled': {
    type: string;
  };
  overscan: {
    type: string;
  };
  'secondminitft.enabled': {
    type: string;
  };
  'secondminitft.type': {
    type: string;
    allowedStringList: string[];
  };
  'secondminitft.resolution': {
    type: string;
    allowedStringList: string[];
  };
  'secondminitft.imagestretchenabled': {
    type: string;
  };
  'secondminitft.imageenlargeenabled': {
    type: string;
  };
  'secondminitft.imagealphaenabled': {
    type: string;
  };
  'secondminitft.imageignoreaspectenabled': {
    type: string;
  };
  'secondminitft.disablevideoines': {
    type: string;
  };
  'secondminitft.backlightcontrol': {
    type: string;
    lowerValue: number;
    higherValue: number;
  };
  'secondminitft.usemarquee': {
    type: string;
  };
  'secondminitft.sleepenabled': {
    type: string;
  };
  'externalscreen.prefered': {
    type: string;
    allowedChars: string;
  };
  'externalscreen.forceresolution': {
    type: string;
    allowedChars: string;
  };
  'externalscreen.forcefrequency': {
    type: string;
    allowedChars: string;
  };
}
