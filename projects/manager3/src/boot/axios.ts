/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { boot } from 'quasar/wrappers';
import axios, { AxiosInstance } from 'axios';
import { Loading, Notify } from 'quasar';
import { BIOS, MEDIA, SYSTEM } from 'src/router/api.routes';
import { i18n } from 'boot/i18n';

declare module '@vue/runtime-core' {
  interface ComponentCustomProperties {
    $axios: AxiosInstance;
  }
}

let clientApiUrl = process.env.API_URL;
let clientApi80Url = process.env.API_URL80;

if (process.env.API_URL === '/api') {
  clientApiUrl = `${window.location.protocol}//${window.location.host}:81${process.env.API_URL}`;
  clientApi80Url = `${window.location.protocol}//${window.location.host}${process.env.API_URL}`;
}

const apiUrl = clientApiUrl;

// Be careful when using SSR for cross-request state pollution
// due to creating a Singleton instance here;
// If any client changes this (global) instance, it might be a
// good idea to move this instance creation inside of the
// "export default () => {}" function below (which runs individually
// for each client)
const api:AxiosInstance = axios.create({ baseURL: clientApiUrl });
const api80:AxiosInstance = axios.create({ baseURL: clientApi80Url });
const httpClient:AxiosInstance = axios.create();

export default boot(({ app }):void => {
  // for use inside Vue files (Options API) through this.$axios and this.$api

  app.config.globalProperties.$axios = axios;
  // ^ ^ ^ this will allow you to use this.$axios (for Vue Options API form)
  //       so you won't necessarily have to import axios in each vue file

  app.config.globalProperties.$api = api;
  // ^ ^ ^ this will allow you to use this.$api (for Vue Options API form)
  //       so you can easily perform requests against your app's API

  app.config.globalProperties.$api80 = api80;
  // ^ ^ ^ this will allow you to use this.$api80 (for Vue Options API form)
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
  let message:string = i18n.global.t('general.notify.updateSuccess');
  let icon = 'mdi-check-bold';
  Loading.hide();

  if (response.config.url === MEDIA.takeScreenshot) {
    message = i18n.global.t('general.notify.screenshotTaken');
    icon = 'mdi-folder-multiple-image';
  }

  // eslint-disable-next-line @typescript-eslint/ban-ts-comment
  // @ts-ignore
  if (response.config.method === 'delete' && response.config.url.includes(MEDIA.delete)) {
    message = i18n.global.t('general.notify.screenshotDeleted');
    icon = 'mdi-image-remove';
  }

  if (response.config.method === 'post' || response.config.method === 'delete') {
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

api80.interceptors.request.use((config) => {
  // change some state in your store here
  // Do something before request is sent
  Loading.show();

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
api80.interceptors.response.use((response) => {
  let message:string = i18n.global.t('general.notify.updateSuccess');
  let icon = 'mdi-check-bold';
  let timeout = 5000;
  let closeBtn = false;
  let html = false;

  Loading.hide();

  if (response.config.url === SYSTEM.es.start) {
    message = i18n.global.t('general.notify.esStart');
    icon = 'mdi-play';
  }
  if (response.config.url === SYSTEM.es.stop) {
    message = i18n.global.t('general.notify.esStop');
    icon = 'mdi-stop';
  }
  if (response.config.url === SYSTEM.es.restart) {
    message = i18n.global.t('general.notify.esRestart');
    icon = 'mdi-restart';
  }
  if (response.config.url === SYSTEM.supportArchive) {
    message = i18n.global.t('general.notify.supportArchive1') + response.data.linkResponse + i18n.global.t('general.notify.supportArchive2');
    icon = 'mdi-check-bold';
    timeout = 0;
    closeBtn = true;
    html = true;
  }

  if (response.config.method === 'get' || response.config.method === 'post') {
    Notify.create({
      message,
      type: 'positive',
      icon,
      timeout,
      closeBtn,
      html,
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

export {
  api,
  api80,
  apiUrl,
  httpClient,
};
