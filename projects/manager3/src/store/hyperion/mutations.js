import Vue from 'vue'

export function OPTIONS_HYPERION(state, data) {
  state._options = data
}

export function GET_HYPERION(state, data) {
  Object.keys(data).map(key => {
    Vue.set(state, key, data[key])
  })
}
