import {format} from 'quasar'
import Vue from 'vue'
import tools from '../../tools/index'

export function OPTIONS_SYSTEM(state, data) {
  let result = data,
    languages = Object.values(data.language.allowedStringList),
    languagesTranslations = Object.values(data.language.displayableStringList),
    kblayouts = Object.values(data.kblayout.allowedStringList),
    kblayoutsTranslations = Object.values(data.kblayout.displayableStringList),
    timezones = Object.values(data.timezone.allowedStringList).sort(),
    specialkeys = Object.values(data['emulators.specialkeys'].allowedStringList).sort()

  // Construction of the languages options list
  languages = Object.keys(languages).map( key => {
    return {label: format.capitalize((languagesTranslations[key]).toLowerCase()), value: languages[key]}
  }).sort(tools.compare)

  // Construction of the kblayouts options list
  kblayouts = Object.keys(kblayouts).map( key => {
    return {label: format.capitalize((kblayoutsTranslations[key]).toLowerCase()), value: kblayouts[key]}
  }).sort(tools.compare)

  // Construction of the timezones options list
  timezones = Object.keys(timezones).map( key => {
    return {label: timezones[key], value: timezones[key]}
  })

  // Construction of the specialkeys options list
  specialkeys = Object.keys(specialkeys).map( key => {
    return {label: specialkeys[key], value: specialkeys[key]}
  })

  // State mapping
  result.language.allowedStringList = languages
  result.kblayout.allowedStringList = kblayouts
  result.timezone.allowedStringList = timezones
  result['emulators.specialkeys'].allowedStringList = specialkeys
  state._options = result
}

export function GET_SYSTEM(state, data) {
  Object.keys(data).map(key => {
    Vue.set(state, key, data[key])
  })
}
