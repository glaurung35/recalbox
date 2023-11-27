/**
 * @author Nicolas TESSIER aka Asthonishia
 */

export interface MediasResponse {
  mediaPath: string;
  mediaList: Record<string, {
    type: string;
  }>;
}
