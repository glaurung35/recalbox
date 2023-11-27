/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { api80 } from 'boot/axios';
import { AxiosInstance } from 'axios';

// eslint-disable-next-line consistent-return
const Api80ProviderStorePlugin = (): {_api80Provider: AxiosInstance} => ({
  _api80Provider: api80,
});

export default Api80ProviderStorePlugin;
