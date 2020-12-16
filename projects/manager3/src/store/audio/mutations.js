import Vue from 'vue'

export function OPTIONS_AUDIO(state, data) {
  let result = data,
    devices = Object.values(data.device.allowedStringList).sort()

  // Construction of the types options list
  devices = Object.keys(devices).map( key => {
    return {label: devices[key], value: devices[key]}
  })

  // State mapping
  result.device.allowedStringList = devices
  state._options = result
}

export function SET_AUDIO(state, data) {
  Object.keys(data).map(key => {
    Vue.set(state, key, data[key])
  })
}
