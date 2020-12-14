import Vue from 'vue'

export function SET_MONITORING_STORAGE_INFO(state, data) {
  state.storageInfo = data
}

export function SET_MONITORING_METRICS(state, data) {
  let temperatures = []

  if (!state.metrics) {
    temperatures.push([(new Date(data.timestamp)).getTime(), data.temperature.temperatures[0].toFixed(1)])
  } else {
    temperatures = state.metrics.temperature.temperatures[0].data
    temperatures.push(
      [(new Date(data.timestamp)).getTime(), data.temperature.temperatures[0].toFixed(1)]
    )
  }

  data.temperature.temperatures = [
    {
      data: temperatures.slice(-30)
    }
  ]

  Vue.set(state, 'metrics', data)
}
