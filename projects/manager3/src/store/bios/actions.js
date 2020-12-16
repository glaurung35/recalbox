import {axiosClient} from 'boot/axios'
import {BIOS} from 'src/store/api.routes'

export const get = ({commit}) => {
  axiosClient.get(BIOS.all)
    .then(response => {
      commit('SET_BIOS', response.data)
    })
    .catch(error => console.log(error))
}

export const download = ({commit}) => {
  axiosClient.get(BIOS.export)
  // .then(response => {
  //   commit('SET_BIOS', response.data)
  // })
  // .catch(error => console.log(error))
}

export const remove = ({commit}, data) => {
  axiosClient.delete(BIOS.root, data)
    .then(response => {
      commit('SET_BIOS', response.data)
    })
    .catch(error => console.log(error))
}
