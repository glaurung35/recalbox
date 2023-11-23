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
    'wifi2',
    'wifi3',
    'system',
    'updates',
    'controllers',
    'global',
    'patreon',
    'music',
    'hat',
    'autorun',
  ];

  if (allowedStores.includes(context.store.$id)) {
    return {
      async post(data: Record<string, unknown>):Promise<void> {
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
