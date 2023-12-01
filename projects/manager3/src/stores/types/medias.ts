/**
 * @author Nicolas TESSIER aka Asthonishia
 */
export interface Screenshot {
  name: string;
  path: string;
  date: string;
  type: 'image'|'video';
}

export interface MediasResponse {
  mediaPath: string;
  mediaList: Record<string, {
    type: string;
  }>;
}
