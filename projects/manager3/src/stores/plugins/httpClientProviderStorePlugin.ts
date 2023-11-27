/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { httpClient } from 'boot/axios';
import { AxiosInstance } from 'axios';

// eslint-disable-next-line consistent-return
const HttpClientProviderStorePlugin = (): {_httpClientProvider: AxiosInstance} => ({
  _httpClientProvider: httpClient,
});

export default HttpClientProviderStorePlugin;
