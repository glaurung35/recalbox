import {axiosClient} from 'boot/axios'
import {HYPERION} from 'src/store/api.routes'

export const options = ({commit}) => {
  axiosClient.options(HYPERION)
    .then(response => {
      commit('OPTIONS_HYPERION', response.data)
    })
    .catch(error => console.log(error))
}

export const get = ({commit}) => {
  axiosClient.get(HYPERION)
    .then(response => {
      commit('GET_HYPERION', response.data)
    })
    .catch(error => console.log(error))
}

export const post = ({commit}, data) => {
  axiosClient.post(HYPERION, data)
    .then(response => {
      commit('GET_HYPERION', response.data)
    })
    .catch(error => console.log(error))
}
