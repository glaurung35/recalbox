import {axiosClient} from 'boot/axios'
import {UPDATES} from 'src/store/api.routes'

export const options = ({commit}) => {
  axiosClient.options(UPDATES)
    .then(response => {
      commit('OPTIONS_UPDATES', response.data)
    })
    .catch(error => console.log(error))
}

export const get = ({commit}) => {
  axiosClient.get(UPDATES)
    .then(response => {
      commit('SET_UPDATES', response.data)
    })
    .catch(error => console.log(error))
}

export const post = ({commit}, data) => {
  axiosClient.post(UPDATES, data)
    .then(response => {
      commit('SET_UPDATES', response.data)
    })
    .catch(error => console.log(error))
}
