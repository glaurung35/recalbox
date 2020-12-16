import Vue from 'vue'

export function SET_SYSTEMS(state, data) {
  let systems = []

  Object.keys(data.systemList).map(systemKey => {
    return systems.push(data.systemList[systemKey])
  })

  Object.keys(systems).map(key => {
    Vue.set(state, key, systems[key])
  })
}
