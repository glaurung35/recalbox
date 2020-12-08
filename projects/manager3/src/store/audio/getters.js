export function _options(state) {
  return state._options
}

export function bgmusic (state) {
  return state.bgmusic
}

export function deviceOptions(state) {
  return Object.values(state['_options'].device.allowedStringList)
}

export function device (state) {
  return state.device
}

export function volume (state) {
  return state.volume
}
