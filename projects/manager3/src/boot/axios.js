import axios from 'axios'
import {i18n} from 'boot/i18n'
import {Loading, Notify} from 'quasar'
import {BIOS} from 'src/store/api.routes'

let axiosConfig = axios.create({
  baseURL: process.env.API_URL,
});

axiosConfig.interceptors.request.use(config => {
  // change some state in your store here
  // Do something before request is sent
  if (config.url === BIOS.all) {
    Loading.show({
      message: i18n.t('emulation.bios.loader'),
    })
  } else {
    Loading.show()
  }

  return config
}, error => {
  // Do something with request error
  Notify.create({
    message: error,
    type: 'negative',
    icon: 'mdi-alert-outline',
  })

  Loading.hide()
  return Promise.reject(error)
})

axiosConfig.interceptors.response.use(response => {
  // Do something with response data
  // call some store functions etc..
  Loading.hide()

  if (response.config.method === 'post') {
    Notify.create({
      message: i18n.t('general.notify.updateSuccess'),
      type: 'positive',
      icon: 'mdi-check-bold',
    })
  }

  return response
}, error => {
  let message = error

  // some network errors can't be detected so...
  if (!error.status) {
    message = 'Network error'
  }

  Notify.create({
    message: i18n.t('general.notify.updateError') + message,
    type: 'negative',
    icon: 'mdi-check-bold',
  })

  Loading.hide()
  // Do something with response error
  return Promise.reject(error)
})

export const axiosClient = axiosConfig
