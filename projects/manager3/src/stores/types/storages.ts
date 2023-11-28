/**
 * @author Nicolas TESSIER aka Asthonishia
 */

export interface StoragesResponse {
  storages: Record<string, {
    recalbox: string;
    devicebus: string;
    devicetype: string;
    devicemodel: string;
    mount: string;
    filesystem: string;
    filesystemtype: string;
    size: number;
    used: number;
  }>;
}
