/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { store } from 'quasar/wrappers';
import { createPinia } from 'pinia';
import { Router } from 'vue-router';
import FetchStorePlugin from 'stores/plugins/fetchStorePlugin';
import FetchOptionsStorePlugin from 'stores/plugins/fetchOptionsStorePlugin';
import PostStorePlugin from 'stores/plugins/postStorePlugin';
import ApiProviderStorePlugin from 'stores/plugins/apiProviderStorePlugin';
import Api80ProviderStorePlugin from 'stores/plugins/api80ProviderStorePlugin';
import HttpClientProviderStorePlugin from 'stores/plugins/httpClientProviderStorePlugin';

/*
 * When adding new properties to stores, you should also
 * extend the `PiniaCustomProperties` interface.
 * @see https://pinia.vuejs.org/core-concepts/plugins.html#typing-new-store-properties
 */
declare module 'pinia' {
  export interface PiniaCustomProperties {
    readonly router: Router;
  }
}

/*
 * If not building with SSR mode, you can
 * directly export the Store instantiation;
 *
 * The function below can be async too; either use
 * async/await or return a Promise which resolves
 * with the Store instance.
 */

export default store((/* { ssrContext } */) => {
  const pinia = createPinia();

  // eslint-disable-next-line @typescript-eslint/ban-ts-comment
  // @ts-ignore
  pinia.use(ApiProviderStorePlugin);
  // eslint-disable-next-line @typescript-eslint/ban-ts-comment
  // @ts-ignore
  pinia.use(Api80ProviderStorePlugin);
  // eslint-disable-next-line @typescript-eslint/ban-ts-comment
  // @ts-ignore
  pinia.use(HttpClientProviderStorePlugin);
  // eslint-disable-next-line @typescript-eslint/ban-ts-comment
  // @ts-ignore
  pinia.use(FetchStorePlugin);
  // eslint-disable-next-line @typescript-eslint/ban-ts-comment
  // @ts-ignore
  pinia.use(FetchOptionsStorePlugin);
  // eslint-disable-next-line @typescript-eslint/ban-ts-comment
  // @ts-ignore
  pinia.use(PostStorePlugin);

  return pinia;
});
