import Vue from 'vue'
import Vuex from 'vuex'
import audio from './audio'
import bios from './bios'
import controllers from './controllers'
import emulationstation from './emulationstation'
import global from './global'
import hyperion from './hyperion'
import kodi from './kodi'
import monitoring from './monitoring'
import roms from './roms'
import scraper from './scraper'
import screenshots from './screenshots'
import system from './system'
import systems from './systems'
import updates from './updates'
import versions from './versions'
import wifi from './wifi'

Vue.use(Vuex)

/*
 * If not building with SSR mode, you can
 * directly export the Store instantiation;
 *
 * The function below can be async too; either use
 * async/await or return a Promise which resolves
 * with the Store instance.
 */

const Store = new Vuex.Store({
  debug: true,
  modules: {
    audio,
    bios,
    controllers,
    emulationstation,
    global,
    hyperion,
    kodi,
    monitoring,
    roms,
    scraper,
    screenshots,
    system,
    systems,
    updates,
    versions,
    wifi
  },

  // enable strict mode (adds overhead!)
  // for dev mode only
  strict: process.env.DEV,
})

/*
 if we want some HMR magic for it, we handle
 the hot update like below. Notice we guard this
 code with "process.env.DEV" -- so this doesn't
 get into our production build (and it shouldn't).
 */
if (process.env.DEV && module.hot) {
  module.hot.accept([
    './audio',
    './bios',
    './controllers',
    './emulationstation',
    './global',
    './hyperion',
    './kodi',
    './monitoring',
    './roms',
    './scraper',
    './screenshots',
    './system',
    './systems',
    './updates',
    './versions',
    './wifi',
  ], () => {
    const newAudio = require('./audio').default
    const newBios = require('./bios').default
    const newControllers = require('./controllers').default
    const newEmulationStation = require('./emulationstation').default
    const newGlobal = require('./global').default
    const newHyperion = require('./hyperion').default
    const newKodi = require('./kodi').default
    const newMonitoring = require('./monitoring').default
    const newRoms = require('./roms').default
    const newScraper = require('./scraper').default
    const newScreenshots = require('./screenshots').default
    const newSystem = require('./system').default
    const newSystems = require('./systems').default
    const newUpdates = require('./updates').default
    const newVersions = require('./versions').default
    const newWifi = require('./wifi').default

    Store.hotUpdate({
      modules: {
        audio: newAudio,
        bios: newBios,
        controllers: newControllers,
        emulationstation: newEmulationStation,
        global: newGlobal,
        hyperion:newHyperion,
        kodi: newKodi,
        monitoring: newMonitoring,
        roms: newRoms,
        scraper: newScraper,
        screenshots: newScreenshots,
        system: newSystem,
        systems: newSystems,
        updates: newUpdates,
        versions: newVersions,
        wifi: newWifi
      },
    })
  })
}

export default Store
