import {axiosClient} from 'boot/axios'
import {SCREENSHOTS} from 'src/store/api.routes'

export const get = ({commit}) => {
  axiosClient.get(SCREENSHOTS)
    .then(response => {
      commit('GET_SCREENSHOTS', response.data)
    })
    .catch(error => console.log(error))
}

export const remove = ({commit}, data) => {
  axiosClient.delete(SCREENSHOTS, data)
    .then(response => {
      commit('GET_SCREENSHOTS', response.data)
    })
    .catch(error => console.log(error))
}

