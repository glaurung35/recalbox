import Vue from 'vue'

export function GET_MONITORING_STORAGE_INFO(state, data) {
  state.storageInfo = data
}

export function UPDATE_MONITORING(state, data) {
  let temperature = state.metrics ? [
    ...state.metrics.temperature,
    data.temperature.temperatures[0]
  ] : [data.temperature.temperatures[0]]

  data = {
    cpus: {

    },
    memory: {
      available: data.memory.available[0],
      free: data.memory.free[0],
      total: data.memory.total
    },
    temperatures: temperature,
    timestamp: data.timestamp
  }
  Vue.set(state, 'metrics', data)
}
