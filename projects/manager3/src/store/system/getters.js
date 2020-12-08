// SYSTEM
export function _options(state) {
  return state._options
}

export function timezoneOptions(state) {
  return Object.values(state['_options'].timezone.allowedStringList)
}

export function timezone (state) {
  return state.timezone
}

export function languageOptions(state) {
  return Object.values(state['_options'].language.allowedStringList)
}

export function language (state) {
  return state.language
}

export function kblayoutOptions(state) {
  return Object.values(state['_options'].kblayout.allowedStringList)
}

export function kblayout (state) {
  return state.kblayout
}

export function specialkeysOptions(state) {
  return Object.values(state['_options']['emulators.specialkeys'].allowedStringList)
}

export function specialkeys (state) {
  return state['emulators.specialkeys']
}

// NETWORK
export function hostname (state) {
  return state.hostname
}
