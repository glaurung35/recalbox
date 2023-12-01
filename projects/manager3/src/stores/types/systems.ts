/**
 * @author Nicolas TESSIER aka Asthonishia
 */

export interface System {
  name: string;
  fullname: string;
  romFolder: string;
  themeFolder: string;
  extensions: string[];
  command: string;
  emulators: {
    libretro: Record<string, string>;
  }
}

export interface SystemsResponse {
  romPath: string;
  systemList: Record<string, System>;
}
