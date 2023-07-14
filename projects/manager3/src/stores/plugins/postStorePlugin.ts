/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { PiniaPluginContext } from 'pinia';

// eslint-disable-next-line consistent-return
const PostStorePlugin = (context: PiniaPluginContext) => {
  const allowedStores:string[] = [
    'audio',
    'hyperion',
    'kodi',
    'scraper',
    'emulationstation',
    'wifi',
    'system',
    'updates',
    'controllers',
    'global',
  ];

  if (allowedStores.includes(context.store.$id)) {
    return {
      async post(data: Record<string, unknown>) {
        try {
          const response = await context.store._apiProvider.post(context.store._baseUrl, data);
          context.store[context.store.$id] = response.data;
        } catch (error) {
          // eslint-disable-next-line no-console
          console.log(error);
        }
      },
    };
  }
};

export default PostStorePlugin;
