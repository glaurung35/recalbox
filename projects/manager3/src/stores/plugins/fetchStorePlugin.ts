import { PiniaPluginContext } from 'pinia';

// eslint-disable-next-line consistent-return
const FetchStorePlugin = (context: PiniaPluginContext) => {
  const allowedStores = [
    'audio',
    'hyperion',
    'kodi',
    'scraper',
    'emulationstation',
    'wifi',
    'system',
    'updates',
    'controllers',
    'versions',
    'bios',
    'global',
    'systems',
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
