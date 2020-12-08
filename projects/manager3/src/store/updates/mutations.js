import Vue from 'vue'

export function OPTIONS_UPDATES(state, data) {
  let result = data,
    types = Object.values(data.type.allowedStringList).sort()

  // Construction of the types options list
  types = Object.keys(types).map( key => {
    return {label: types[key], value: types[key]}
  })

  // State mapping
  result.type.allowedStringList = types
  state._options = result
}

export function GET_UPDATES(state, data) {
  Object.keys(data).map(key => {
    Vue.set(state, key, data[key])
  })
}
