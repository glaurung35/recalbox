import {axiosClient} from 'boot/axios'
import {MONITORING} from 'src/store/api.routes'

export const getStorageInfo = ({commit}) => {
  axiosClient.get(MONITORING.storageInfo)
    .then(response => {
      commit('GET_MONITORING_STORAGE_INFO', response.data)
    })
    .catch(error => console.log(error))
}

export const update = ({commit}, data) => {
  commit('UPDATE_MONITORING', data)
}
