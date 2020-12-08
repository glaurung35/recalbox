import {axiosClient} from 'boot/axios'
import {GLOBAL} from 'src/store/api.routes'

export const options = ({commit}) => {
  axiosClient.options(GLOBAL)
    .then(response => {
      commit('OPTIONS_GLOBAL', response.data)
    })
    .catch(error => console.log(error))
}

export const get = ({commit}) => {
  axiosClient.get(GLOBAL)
    .then(response => {
      commit('GET_GLOBAL', response.data)
    })
    .catch(error => console.log(error))
}

export const post = ({commit}, data) => {
  axiosClient.post(GLOBAL, data)
    .then(response => {
      commit('GET_GLOBAL', response.data)
    })
    .catch(error => console.log(error))
}
