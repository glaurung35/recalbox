import Vue from 'vue'

export function OPTIONS_EMUSTATION(state, data) {
  let result = data,
    menus = Object.values(data.menu.allowedStringList).sort(),
    selectedsystems = Object.values(data.selectedsystem.allowedStringList).sort()

  // Construction of the types options list
  menus = Object.keys(menus).map( key => {
    return {label: menus[key], value: menus[key]}
  })

  // Construction of the selectedsystems options list
  selectedsystems = Object.keys(selectedsystems).map( key => {
    return {label: selectedsystems[key], value: selectedsystems[key]}
  })

  // State mapping
  result.menu.allowedStringList = menus
  result.selectedsystem.allowedStringList = selectedsystems
  state._options = result
}

export function GET_EMUSTATION(state, data) {
  Object.keys(data).map(key => {
    Vue.set(state, key, data[key])
  })
}
