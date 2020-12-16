import {axiosClient} from 'boot/axios'
import {SYSTEMS} from 'src/store/api.routes'

export const get = ({commit}) => {
  axiosClient.get(SYSTEMS.all)
    .then(response => {
      commit('SET_SYSTEMS', response.data)
    })
    .catch(error => console.log(error))
}
