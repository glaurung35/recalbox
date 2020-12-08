import {LocalStorage} from 'quasar'
import messages from 'src/i18n'
import Vue from 'vue'
import VueI18n from 'vue-i18n'

Vue.use(VueI18n)

if (!LocalStorage.getItem('locale')) {
  try {
    LocalStorage.set('locale', 'fr-fr')
  } catch (e) {
    console.log(e)
  }
}

const i18n = new VueI18n({
  locale: LocalStorage.getItem('locale'),
  fallbackLocale: 'en-us',
  messages,
})

export default ({app}) => {
  // Set i18n instance on app
  app.i18n = i18n
}

export {i18n}
