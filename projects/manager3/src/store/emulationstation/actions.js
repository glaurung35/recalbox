import {axiosClient} from 'boot/axios'
import {EMUSTATION} from 'src/store/api.routes'

export const options = ({commit}) => {
  axiosClient.options(EMUSTATION)
    .then(response => {
      commit('OPTIONS_EMUSTATION', response.data)
    })
    .catch(error => console.log(error))
}

export const get = ({commit}) => {
  axiosClient.get(EMUSTATION)
    .then(response => {
      commit('SET_EMUSTATION', response.data)
    })
    .catch(error => console.log(error))
}

export const post = ({commit}, data) => {
  axiosClient.post(EMUSTATION, data)
    .then(response => {
      commit('SET_EMUSTATION', response.data)
    })
    .catch(error => console.log(error))
}
