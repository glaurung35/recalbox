/**
 * @author Nicolas TESSIER aka Asthonishia
 */

export interface SystemsResponse {
  romPath: string;
  systemList: Record<string, {
    name: string;
    fullname: string;
    romFolder: string;
    themeFolder: string;
    extensions: string[];
    command: string;
    emulators: {
      libretro: Record<string, string>;
    }
  }>;
}
