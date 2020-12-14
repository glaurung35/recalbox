import {i18n} from 'boot/i18n'
import Vue from 'vue'

export function GET_MONITORING_STORAGE_INFO(state, data) {
  state.storageInfo = data
}

export function UPDATE_MONITORING(state, data) {
  // Cores data definition
  let cores = [
    { name: i18n.t('monitoring.cores.used'), data: [] },
  ]

  if(data) {
    Object.keys(data.cpus).map(key => {
      cores[0].data.push(
        { x: 'Core ' + (parseInt(key) + 1), y: data.cpus[key].consumption[0].toFixed(0) }
      )
    })
  }

  // Temperatures data definition
  let temperatures = null

  if(state.metrics) {
    temperatures = state.metrics.temperatures[0].data
    temperatures.push(
      [
        (new Date(data.timestamp)).getTime(), data.temperature.temperatures[0].toFixed(1)
      ]
    )
  } else {
    temperatures = [
      [
        (new Date(data.timestamp)).getTime(), data.temperature.temperatures[0].toFixed(1)
      ]
    ]
  }

  data = {
    temperatures: [{
      data: temperatures.slice(-30)
    }],
    cores: cores,
    memory: {
      available: data.memory.available[0],
      free: data.memory.free[0],
      total: data.memory.total
    }
  }

  Vue.set(state, 'metrics', data)
}
