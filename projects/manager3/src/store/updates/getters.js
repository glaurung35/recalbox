export function _options (state) {
  return state._options
}

export function enabled (state) {
  return state.enabled
}

export function typeOptions(state) {
  return Object.values(state['_options'].type.allowedStringList)
}

export function type (state) {
  return state.type
}
