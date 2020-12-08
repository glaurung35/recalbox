import Vue from 'vue'

export function OPTIONS_KODI(state, data) {
  state._options = data
}

export function GET_KODI(state, data) {
  Object.keys(data).map(key => {
    Vue.set(state, key, data[key])
  })
}
