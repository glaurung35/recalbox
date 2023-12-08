/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { PiniaPluginContext } from 'pinia';

export interface FetchStore {
  fetch(): Promise<void>;
}

// eslint-disable-next-line consistent-return
const FetchStorePlugin = (context: PiniaPluginContext) => {
  const allowedStores = [
    'architecture',
    'audio',
    'autorun',
    'bios',
    'controllers',
    'emulationstation',
    'global',
    'hat',
    'hyperion',
    'kodi',
    'media',
    'monitoring',
    'music',
    'patreon',
    'scraper',
    'system',
    'systems',
    'tate',
    'updates',
    'versions',
    'wifi',
    'wifi2',
    'wifi3',
    'system',
    'updates',
  ];

  if (allowedStores.includes(context.store.$id)) {
    return {
      async fetch() {
        try {
          const response = await context.store._apiProvider.get(context.store._baseUrl);
          context.store[context.store.$id] = response.data;
        } catch (error) {
          // eslint-disable-next-line no-console
          console.log(error);
        }
      },
    };
  }
};

export default FetchStorePlugin;
