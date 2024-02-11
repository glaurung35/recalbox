/**
 * @author Nicolas TESSIER aka Asthonishia
 */

export interface Storage {
  recalbox: string;
  devicebus: string;
  devicetype: string;
  devicemodel: string;
  mount: string;
  filesystem: string;
  filesystemtype: string;
  size: number;
  used: number;
}

export interface StoragesResponse {
  storages: Record<string, Storage>;
}
