import {axiosClient} from 'boot/axios'
import {VERSIONS} from 'src/store/api.routes'

export const get = ({commit}) => {
  axiosClient.get(VERSIONS)
    .then(response => {
      commit('SET_VERSIONS', response.data)
    })
    .catch(error => console.log(error))
}
