export function _options (state) {
  return state._options
}

export function ratioOptions(state) {
  return Object.values(state['_options'].ratio.allowedStringList)
}

export function ratio (state) {
  return state.ratio
}

export function rewind (state) {
  return state.rewind
}

export function autosave (state) {
  return state.autosave
}

export function shadersetOptions(state) {
  return Object.values(state['_options']['shaderset'].allowedStringList)
}

export function shaderset(state) {
  return state.shaderset
}

export function shadersetfileOptions(state) {
  return Object.values(state['_options']['shaderset.file'].allowedStringList)
}

export function shadersetfile(state) {
  return state['shaderset.file']
}

export function integerscale(state) {
  return state.integerscale
}

export function retroachievements(state) {
  return state.retroachievements
}

export function username(state) {
  return state['retroachievements.username']
}

export function password(state) {
  return state['retroachievements.password']
}

export function hardcore(state) {
  return state['retroachievements.hardcore']
}
