export function _options(state) {
  return state._options
}

export function db9(state) {
  return state['db9.enabled']
}

export function db9args(state) {
  return state['db9.args']
}

export function gamecon(state) {
  return state['gamecon.enabled']
}

export function gameconargs(state) {
  return state['gamecon.args']
}

export function gpio(state) {
  return state['gpio.enabled']
}

export function gpioargs(state) {
  return state['gpio.args']
}

export function ps3(state) {
  return state['ps3.enabled']
}

export function ps3driverOptions(state) {
  return Object.values(state['_options']['ps3.driver'].allowedStringList)
}

export function ps3driver(state) {
  return state['ps3.driver']
}
