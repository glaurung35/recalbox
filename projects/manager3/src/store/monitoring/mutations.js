import Vue from 'vue'

export function GET_MONITORING_STORAGE_INFO(state, data) {
  state.storageInfo = data
}

export function UPDATE_MONITORING(state, data) {
  let temperatures = null

  if(state.metrics) {
    temperatures = state.metrics.temperatures[0].data
    temperatures.push(
      [
        (new Date(data.timestamp)).getTime(),
        data.temperature.temperatures[0]
      ]
    )
  } else {
    temperatures = [
      [
        (new Date(data.timestamp)).getTime(),
        data.temperature.temperatures[0]
      ]
    ]
  }

  data = {
    cpus: {

    },
    memory: {
      available: data.memory.available[0],
      free: data.memory.free[0],
      total: data.memory.total
    },
    temperatures: [{
      name: 'C°',
      data: temperatures.slice(-60)
    }],
    timestamp: data.timestamp
  }
  Vue.set(state, 'metrics', data)
}
