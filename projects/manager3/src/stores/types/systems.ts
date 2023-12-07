/**
 * @author Nicolas TESSIER aka Asthonishia
 */

export interface Inputs {
  pads: number;
  keyboard: number;
  mouse: number;
}

export interface Properties {
  hasLightgunSupport: boolean;
  isReadOnly: boolean;
  hasNetplay: boolean;
}

export interface System {
  name: string;
  fullName: string;
  uuid: string;
  manufacturer: string;
  releaseDate: number;
  romPath: string[];
  extensions: string;
  type: number;
  ignoredFiles: string;
  inputs: Inputs;
  properties: Properties;
  emulators: string[];
  themeFolder: string;
}

export const Type = {
  0: 'Unknown',
  1: 'Arcade',
  2: 'Home Console',
  3: 'Handheld Console',
  4: 'Computer',
  5: 'Fantasy Console',
  6: 'Game Engine',
  7: 'Port',
  8: 'Virtual System',
  9: 'Virtual Arcade',
};

export type SystemType = Record<string, string>

export type DeviceRequirement = Record<string, string>

export type EmulatorSpeed = Record<string, string>

export type EmulatorCompatibility = Record<string, string>

export interface Enumerations {
  systemTypes: SystemType[];
  deviceRequirement: DeviceRequirement[];
  emulatorSpeed: EmulatorSpeed[];
  emulatorCompatibility: EmulatorCompatibility[];
}

export interface SystemsResponse {
  enumerations: Enumerations;
  systems: System[]|[];
}
