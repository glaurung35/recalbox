/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { api } from 'boot/axios';
import { AxiosInstance } from 'axios';

export interface ApiProviderStore {
  _apiProvider: AxiosInstance;
}

// eslint-disable-next-line consistent-return
const ApiProviderStorePlugin = (): {_apiProvider: AxiosInstance} => ({
  _apiProvider: api,
});

export default ApiProviderStorePlugin;
