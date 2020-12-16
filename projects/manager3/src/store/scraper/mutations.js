import Vue from 'vue'

export function OPTIONS_SCRAPER(state, data) {
  state._options = data
}

export function SET_SCRAPER(state, data) {
  Object.keys(data).map(key => {
    Vue.set(state, key, data[key])
  })
}
