/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { httpClient } from 'boot/axios';
import { AxiosInstance } from 'axios';

export interface HttpClientProviderStore {
  _httpClientProvider: AxiosInstance;
}

// eslint-disable-next-line consistent-return
const HttpClientProviderStorePlugin = (): {_httpClientProvider: AxiosInstance} => ({
  _httpClientProvider: httpClient,
});

export default HttpClientProviderStorePlugin;
