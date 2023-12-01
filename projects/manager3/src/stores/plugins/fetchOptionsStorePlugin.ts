/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { PiniaPluginContext } from 'pinia';

export interface FetchOptionsStore {
  fetchOptions(): Promise<void>;
}

// eslint-disable-next-line consistent-return
const FetchOptionsStorePlugin = (context: PiniaPluginContext) => {
  const allowedStores = [
    'audio',
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
    'hat',
    'autorun',
  ];

  if (allowedStores.includes(context.store.$id)) {
    return {
      async fetchOptions() {
        try {
          const response = await context.store._apiProvider.options(context.store._baseUrl);
          context.store[`_${context.store.$id}Options`] = response.data;
        } catch (error) {
          // eslint-disable-next-line no-console
          console.log(error);
        }
      },
    };
  }
};

export default FetchOptionsStorePlugin;
