export function _options(state) {
  return state._options
}

export function menuOptions(state) {
  return Object.values(state['_options'].menu.allowedStringList)
}

export function menu (state) {
  return state.menu
}

export function selectedsystemOptions(state) {
  return Object.values(state['_options'].selectedsystem.allowedStringList)
}

export function selectedsystem (state) {
  return state.selectedsystem
}

export function bootongamelist (state) {
  return state.bootongamelist
}

export function hidesystemview (state) {
  return state.hidesystemview
}

export function gamelistonly (state) {
  return state.gamelistonly
}
