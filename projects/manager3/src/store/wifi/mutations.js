import Vue from 'vue'

export function OPTIONS_WIFI(state, data) {
  state._options = data
}

export function GET_WIFI(state, data) {
  Object.keys(data).map(key => {
    Vue.set(state, key, data[key])
  })
}
