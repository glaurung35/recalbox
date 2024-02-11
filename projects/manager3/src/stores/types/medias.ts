/**
 * @author Nicolas TESSIER aka Asthonishia
 */
export enum Type {
  image= 'image',
  webm = 'video/webm',
  xMsvideo = 'video/x-msvideo',
  mp4 = 'video/mp4'
}

export interface Screenshot {
  name: string;
  path: string;
  date: string;
  type: Type;
}

export interface MediasResponse {
  mediaPath: string;
  mediaList: Record<string, {
    type: string;
  }>;
}
