import {LocalStorage, Quasar} from 'quasar'

export default async () => {
  const langIso = LocalStorage.getItem('locale') ? LocalStorage.getItem('locale') === 'fr-fr' ? 'fr' : 'en-us' : 'en-us'

  try {
    await import(`quasar/lang/${langIso}`)
      .then(lang => {
        Quasar.lang.set(lang.default)
      })
  } catch (err) {
    // Requested Quasar Language Pack does not exist,
    // let's not break the app, so catching error
  }
}
