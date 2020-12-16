import {axiosClient} from 'boot/axios'
import {KODI} from 'src/store/api.routes'

export const options = ({commit}) => {
  axiosClient.options(KODI)
    .then(response => {
      commit('OPTIONS_KODI', response.data)
    })
    .catch(error => console.log(error))
}

export const get = ({commit}) => {
  axiosClient.get(KODI)
    .then(response => {
      commit('SET_KODI', response.data)
    })
    .catch(error => console.log(error))
}

export const post = ({commit}, data) => {
  axiosClient.post(KODI, data)
    .then(response => {
      commit('SET_KODI', response.data)
    })
    .catch(error => console.log(error))
}
