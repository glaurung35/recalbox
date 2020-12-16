import Vue from 'vue'

export function OPTIONS_GLOBAL(state, data) {
  let result = data,
    ratios = Object.values(data.ratio.allowedStringList).sort(),
    shadersets = Object.values(data.shaderset.allowedStringList).sort(),
    shadersetsfiles = Object.values(data['shaderset.file'].allowedStringList).sort()

  // Construction of the ratios options list
  ratios = Object.keys(ratios).map( key => {
    return {label: ratios[key], value: ratios[key]}
  })

  // Construction of the ratios options list
  shadersets = Object.keys(shadersets).map( key => {
    return {label: shadersets[key], value: shadersets[key]}
  })

  // Construction of the ratios options list
  shadersetsfiles = Object.keys(shadersetsfiles).map( key => {
    return {label: shadersetsfiles[key], value: shadersetsfiles[key]}
  })

  // State mapping
  result.ratio.allowedStringList = ratios
  result.shaderset.allowedStringList = shadersets
  result['shaderset.file'].allowedStringList = shadersetsfiles
  state._options = result
}

export function SET_GLOBAL(state, data) {
  Object.keys(data).map(key => {
    Vue.set(state, key, data[key])
  })
}
