export function storageInfo (state) {
  return state.storageinfo
}

export function temperatures (state) {
  if (!state.metrics) return [
    { data: [] }
  ]

  return state.metrics.temperature.temperatures
}

export function cores (state) {
  if (!state.metrics) return [
    { data: [{x: 'Core 1', y: 0 }, {x: 'Core 2', y: 0 }, {x: 'Core 3', y: 0 }, {x: 'Core 4', y: 0 }] }
  ]

  let cores = [
    { data: [] }
  ]

  Object.keys(state.metrics.cpus).map(key => {
    cores[0].data.push(
      {
        x: 'Core ' + (parseInt(key) + 1),
        y: state.metrics.cpus[key].consumption[0].toFixed(0)
      }
    )
  })

  return cores
}
