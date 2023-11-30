/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { PiniaPluginContext } from 'pinia';

// eslint-disable-next-line consistent-return
const PostStorePlugin = (context: PiniaPluginContext) => {
  const allowedStores = [
    'audio',
    'autorun',
    'controllers',
    'emulationstation',
    'global',
    'hat',
    'hyperion',
    'kodi',
    'music',
    'patreon',
    'scraper',
    'system',
    'updates',
    'wifi',
    'wifi2',
    'wifi3',
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
