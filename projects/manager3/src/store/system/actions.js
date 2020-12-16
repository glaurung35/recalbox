import {axiosClient} from 'boot/axios'
import {SYSTEM} from 'src/store/api.routes'

// SYSTEM
export const options = ({commit}) => {
  axiosClient.options(SYSTEM)
    .then(response => {
      commit('OPTIONS_SYSTEM', response.data)
    })
    .catch(error => console.log(error))
}

export const get = ({commit}) => {
  axiosClient.get(SYSTEM)
    .then(response => {
      commit('SET_SYSTEM', response.data)
    })
    .catch(error => console.log(error))
}

export const post = ({commit}, data) => {
  axiosClient.post(SYSTEM, data)
    .then(response => {
      commit('SET_SYSTEM', response.data)
    })
    .catch(error => console.log(error))
}
