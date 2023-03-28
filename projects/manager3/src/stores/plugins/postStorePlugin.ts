import { api } from 'boot/axios';
import { PiniaPluginContext } from 'pinia';

// eslint-disable-next-line consistent-return
const PostStorePlugin = (context: PiniaPluginContext) => {
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
  ];

  if (allowedStores.includes(context.store.$id)) {
    return {
      async post(data: Record<string, unknown>) {
        try {
          const response = await api.post(context.store._baseUrl, data);
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
