import { api } from 'boot/axios';
import { PiniaPluginContext } from 'pinia';

// eslint-disable-next-line consistent-return
const FetchOptionsStorePlugin = (context: PiniaPluginContext) => {
  const allowedStores = ['audio', 'kodi', 'scraper', 'emulationstation', 'wifi', 'system'];

  if (allowedStores.includes(context.store.$id)) {
    return {
      async fetchOptions() {
        try {
          const response = await api.options(context.store._baseUrl);
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
