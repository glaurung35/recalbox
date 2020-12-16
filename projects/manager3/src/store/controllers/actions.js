import {axiosClient} from 'boot/axios'
import {CONTROLLERS} from 'src/store/api.routes'

export const options = ({commit}) => {
  axiosClient.options(CONTROLLERS)
    .then(response => {
      commit('OPTIONS_CONTROLLERS', response.data)
    })
    .catch(error => console.log(error))
}

export const get = ({commit}) => {
  axiosClient.get(CONTROLLERS)
    .then(response => {
      commit('SET_CONTROLLERS', response.data)
    })
    .catch(error => console.log(error))
}

export const post = ({commit}, data) => {
  axiosClient.post(CONTROLLERS, data)
    .then(response => {
      commit('SET_CONTROLLERS', response.data)
    })
    .catch(error => console.log(error))
}
