import {axiosClient} from 'boot/axios'
import {ROMS, SYSTEMS} from 'src/store/api.routes'

export const get = ({commit}) => {
  axiosClient.get(ROMS.all)
    .then(response => {
      commit('SET_ROMS', response.data)
    })
    .catch(error => console.log(error))
}

export const getBySystem = ({commit}, system) => {
  axiosClient.get(SYSTEMS.root + '/' + system + '/roms')
    .then(response => {
      commit('SET_ROMS', response.data)
    })
    .catch(error => console.log(error))
}

export const remove = ({commit}, data) => {
  axiosClient.delete(ROMS.root, data)
    .then(response => {
      commit('SET_ROMS', response.data)
    })
    .catch(error => console.log(error))
}
