/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { boot } from 'quasar/wrappers';
import axios, { AxiosInstance } from 'axios';
import { Loading, Notify } from 'quasar';
import { BIOS, MEDIA } from 'src/router/api.routes';
import { i18n } from 'boot/i18n';

declare module '@vue/runtime-core' {
  interface ComponentCustomProperties {
    $axios: AxiosInstance;
  }
}

// Be careful when using SSR for cross-request state pollution
// due to creating a Singleton instance here;
// If any client changes this (global) instance, it might be a
// good idea to move this instance creation inside of the
// "export default () => {}" function below (which runs individually
// for each client)
const api = axios.create({ baseURL: process.env.API_URL });

export default boot(({ app }) => {
  // for use inside Vue files (Options API) through this.$axios and this.$api

  app.config.globalProperties.$axios = axios;
  // ^ ^ ^ this will allow you to use this.$axios (for Vue Options API form)
  //       so you won't necessarily have to import axios in each vue file

  app.config.globalProperties.$api = api;
  // ^ ^ ^ this will allow you to use this.$api (for Vue Options API form)
  //       so you can easily perform requests against your app's API
});

api.interceptors.request.use((config) => {
  // change some state in your store here
  // Do something before request is sent
  if (config.url === BIOS.all) {
    Loading.show({
      message: i18n.global.t('emulation.bios.loader'),
    });
  } else {
    Loading.show();
  }

  return config;
}, (error) => {
  // Do something with request error
  Notify.create({
    message: error,
    type: 'negative',
    icon: 'mdi-alert-outline',
  });

  Loading.hide();
  return Promise.reject(error);
});

// Add Notify Toasters on current axios requests
api.interceptors.response.use((response) => {
  let message = i18n.global.t('general.notify.updateSuccess');
  let icon = 'mdi-check-bold';
  Loading.hide();

  if (response.config.url === MEDIA.takeScreenshot) {
    message = i18n.global.t('general.notify.screenshotTaken');
    icon = 'mdi-folder-multiple-image';
  }

  if (response.config.method === 'post') {
    Notify.create({
      message,
      type: 'positive',
      icon,
    });
  }

  return response;
}, (error) => {
  let message = error;

  // some network errors can't be detected so...
  if (!error.status) {
    message = 'networkError';
  }

  Loading.hide();

  Notify.create({
    message: i18n.global.t(`general.notify.${message}`),
    type: 'negative',
    icon: 'mdi-check-bold',
  });

  return Promise.reject(error);
});

export { api };
