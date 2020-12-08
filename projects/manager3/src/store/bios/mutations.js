import Vue from 'vue'

export function GET_BIOS(state, data) {
  let bios = []

  Object.keys(data).map(systemKey => {
    return Object.keys(data[systemKey].biosList).map(biosKey => {
      let result = data[systemKey].biosList[biosKey]

      result.system = data[systemKey].fullName

      bios.push(result)
    })
  })

  Object.keys(bios).map(key => {
    Vue.set(state, key, bios[key])
  })
}
