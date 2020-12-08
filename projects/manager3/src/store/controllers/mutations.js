import Vue from 'vue'

export function OPTIONS_CONTROLLERS(state, data) {
  let result = data,
    drivers = Object.values(data['ps3.driver'].allowedStringList).sort()

  // Construction of the ratios options list
  drivers = Object.keys(drivers).map( key => {
    return {label: drivers[key], value: drivers[key]}
  })

  // State mapping
  result['ps3.driver'].allowedStringList = drivers
  state._options = result
}

export function GET_CONTROLLERS(state, data) {
  Object.keys(data).map(key => {
    Vue.set(state, key, data[key])
  })
}
