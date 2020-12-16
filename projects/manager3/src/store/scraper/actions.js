import {axiosClient} from 'boot/axios'
import {SCRAPER} from 'src/store/api.routes'

export const options = ({commit}) => {
  axiosClient.options(SCRAPER)
    .then(response => {
      commit('OPTIONS_SCRAPER', response.data)
    })
    .catch(error => console.log(error))
}

export const get = ({commit}) => {
  axiosClient.get(SCRAPER)
    .then(response => {
      commit('SET_SCRAPER', response.data)
    })
    .catch(error => console.log(error))
}

export const post = ({commit}, data) => {
  axiosClient.post(SCRAPER, data)
    .then(response => {
      commit('SET_SCRAPER', response.data)
    })
    .catch(error => console.log(error))
}
