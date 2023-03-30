/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { api } from 'boot/axios';

// eslint-disable-next-line consistent-return
const ApiProviderStorePlugin = () => ({
  _apiProvider: api,
});

export default ApiProviderStorePlugin;
