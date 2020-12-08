import {axiosClient} from 'boot/axios'
import {WIFI} from 'src/store/api.routes'

export const options = ({commit}) => {
  axiosClient.options(WIFI)
    .then(response => {
      commit('OPTIONS_WIFI', response.data)
    })
    .catch(error => console.log(error))
}

export const get = ({commit}) => {
  axiosClient.get(WIFI)
    .then(response => {
      commit('GET_WIFI', response.data)
    })
    .catch(error => console.log(error))
}

export const post = ({commit}, data) => {
  axiosClient.post(WIFI, data)
    .then(response => {
      commit('GET_WIFI', response.data)
    })
    .catch(error => console.log(error))
}
