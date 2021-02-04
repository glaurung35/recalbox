import Vue from 'vue'

export function OPTIONS_KODI(state, data) {
  let result = data,
    videoModes = Object.values(data.videomode.allowedStringList).sort(),
    waitModes = Object.values(data['network.waitmode'].allowedStringList).sort()

  // Construction of the video mode options list
  videoModes = Object.keys(videoModes).map( key => {
    return {label: videoModes[key], value: videoModes[key]}
  })

  // Construction of the wait mode options list
  waitModes = Object.keys(waitModes).map( key => {
    return {label: waitModes[key], value: waitModes[key]}
  })

  // State mapping
  result.videomode.allowedStringList = videoModes
  result['network.waitmode'].allowedStringList = waitModes
  state._options = result
}

export function SET_KODI(state, data) {
  Object.keys(data).map(key => {
    Vue.set(state, key, data[key])
  })
}
