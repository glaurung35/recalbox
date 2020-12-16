import {axiosClient} from 'boot/axios'
import {AUDIO} from 'src/store/api.routes'

export const options = ({commit}) => {
  axiosClient.options(AUDIO)
    .then(response => {
      commit('OPTIONS_AUDIO', response.data)
    })
    .catch(error => console.log(error))
}

export const get = ({commit}) => {
  axiosClient.get(AUDIO)
    .then(response => {
      commit('SET_AUDIO', response.data)
    })
    .catch(error => console.log(error))
}

export const post = ({commit}, data) => {
  axiosClient.post(AUDIO, data)
    .then(response => {
      commit('SET_AUDIO', response.data)
    })
    .catch(error => console.log(error))
}
