export function _options (state) {
  return state._options
}

export function enabled (state) {
  return state.enabled
}

export function atStartup (state) {
  return state.atstartup
}

export function xButton (state) {
  return state.xbutton
}

export function videoModeOptions (state) {
  return Object.values(state['_options'].videomode.allowedStringList)
}

export function videoMode (state) {
  return state.videomode
}

export function waitModeOptions (state) {
  return Object.values(state['_options']['network.waitmode'].allowedStringList)
}

export function waitMode (state) {
  return state['network.waitmode']
}

export function waitTime (state) {
  return state['network.waittime']
}

export function waitHost (state) {
  return state['network.waithost']
}
