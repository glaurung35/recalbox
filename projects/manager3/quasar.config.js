/**
 * @author Nicolas TESSIER aka Asthonishia
 */
/* eslint-env node */

/*
 * This file runs in a Node context (it's NOT transpiled by Babel), so use only
 * the ES6 features that are supported by your Node version. https://node.green/
 */

// Configuration for your app
// https://v2.quasar.dev/quasar-cli-webpack/quasar-config-js

/* eslint-disable @typescript-eslint/no-var-requires */

/* eslint func-names: 0 */
/* eslint global-require: 0 */

require('dotenv').config();

const { configure } = require('quasar/wrappers');

module.exports = configure((ctx) => ({
  // https://v2.quasar.dev/quasar-cli-webpack/supporting-ts
  supportTS: {
    tsCheckerConfig: {
      eslint: {
        enabled: true,
        files: './src/**/*.{ts,tsx,js,jsx,vue}',
      },
    },
  },

  // https://v2.quasar.dev/quasar-cli-webpack/prefetch-feature
  // preFetch: true,

  // app boot file (/src/boot)
  // --> boot files are part of "main.js"
  // https://v2.quasar.dev/quasar-cli-webpack/boot-files
  boot: [
    'i18n',
    'axios',
    'quasar-lang-pack',
    'mqtt',
    'apexcharts',
  ],

  // https://v2.quasar.dev/quasar-cli-webpack/quasar-config-js#Property%3A-css
  css: [
    'app.sass',
  ],

  // https://github.com/quasarframework/quasar/tree/dev/extras
  extras: [
    // 'ionicons-v4',
    'mdi-v6',
    // 'fontawesome-v6',
    // 'eva-icons',
    // 'themify',
    // 'line-awesome',
    // 'roboto-font-latin-ext', // this or either 'roboto-font', NEVER both!

    'roboto-font', // optional, you are not bound to it
    'material-icons', // optional, you are not bound to it
  ],

  // Full list of options: https://v2.quasar.dev/quasar-cli-webpack/quasar-config-js#Property%3A-build
  build: {
    vueRouterMode: 'hash', // available values: 'hash', 'history'
    env: ctx.dev
      ? { // so on dev we'll have
        API_URL: `http://${process.env.API_IP}:81/api`,
        API_URL80: `http://${process.env.API_IP}/api`,

        TWITCH_CLIENT_ID: 'jgp2r9ixktfq9fugog7wjeutk0dt21',
        TWITCH_CLIENT_SECRET: 'dnjccue3una7cccq6hxqpcjtxif69c',

        FORUM_URL: 'https://forum.recalbox.com',
        DISCORD_URL: 'https://discord.gg/NbQFbGM',
        GITLAB_URL: 'https://gitlab.com/recalbox/recalbox',
        PATREON_URL: 'https://www.patreon.com/recalbox',
        TWITCH_URL: 'https://www.twitch.tv/recalbox',
        WIKI_URL: 'https://wiki.recalbox.com',

        MQTT_URL: `mqtt://${process.env.API_IP}:18833`,
        MQTT_MONITORING_CHANNEL: 'Recalbox/WebAPI/SystemInfo',
        MQTT_ES_EVENTS_CHANNEL: 'Recalbox/EmulationStation/EventJson',
        MQTT_VD_EVENTS_KEYBOARD_CHANNEL: 'Recalbox/VirtualDevices/Keyboard',
        MQTT_VD_EVENTS_GAMEPAD_CHANNEL: 'Recalbox/VirtualDevices/GamePad',
        MQTT_VD_EVENTS_TOUCHPAD_CHANNEL: 'Recalbox/VirtualDevices/TouchPad',
      }
      : { // and on build (production):
        API_URL: '/api',
        API_URL80: '',

        TWITCH_CLIENT_ID: 'jgp2r9ixktfq9fugog7wjeutk0dt21',
        TWITCH_CLIENT_SECRET: 'dnjccue3una7cccq6hxqpcjtxif69c',

        FORUM_URL: 'https://forum.recalbox.com',
        DISCORD_URL: 'https://discord.gg/NbQFbGM',
        GITLAB_URL: 'https://gitlab.com/recalbox/recalbox',
        PATREON_URL: 'https://www.patreon.com/recalbox',
        TWITCH_URL: 'https://www.twitch.tv/recalbox',
        WIKI_URL: 'https://wiki.recalbox.com',

        MQTT_URL: '',
        MQTT_MONITORING_CHANNEL: 'Recalbox/WebAPI/SystemInfo',
        MQTT_ES_EVENTS_CHANNEL: 'Recalbox/EmulationStation/EventJson',
      },
    // transpile: false,
    // publicPath: '/',

    // Add dependencies for transpiling with Babel (Array of string/regex)
    // (from node_modules, which are by default not transpiled).
    // Applies only if "transpile" is set to true.
    // transpileDependencies: [],

    // rtl: true, // https://quasar.dev/options/rtl-support
    // preloadChunks: true,
    // showProgress: false,
    // gzip: true,
    // analyze: true,

    // Options below are automatically set depending on the env, set them if you want to override
    // extractCSS: false,

    // https://v2.quasar.dev/quasar-cli-webpack/handling-webpack
    // "chain" is a webpack-chain object https://github.com/neutrinojs/webpack-chain
    chainWebpack(chain) {
      const nodePolyfillWebpackPlugin = require('node-polyfill-webpack-plugin');
      chain.plugin('node-polyfill').use(nodePolyfillWebpackPlugin);
    },
  },

  // Full list of options: https://v2.quasar.dev/quasar-cli-webpack/quasar-config-js#Property%3A-devServer
  devServer: {
    server: {
      type: 'http',
    },
    port: 8080,
    open: true, // opens browser window automatically
  },

  // https://v2.quasar.dev/quasar-cli-webpack/quasar-config-js#Property%3A-framework
  framework: {
    config: {
      loading: {
        spinnerColor: 'accent',
        backgroundColor: 'primary',
        spinnerSize: 140,
        customClass: 'custom-loader',
      },
    },
    lang: 'fr', // Quasar language pack

    // iconSet: 'material-icons', // Quasar icon set
    // lang: 'en-US', // Quasar language pack

    // For special cases outside of where the auto-import strategy can have an impact
    // (like functional components as one of the examples),
    // you can manually specify Quasar components/directives to be available everywhere:
    //
    // components: [],
    directives: [
      'Ripple',
      'ClosePopup',
    ],

    // Quasar plugins
    plugins: [
      'Notify',
      'Loading',
      'LocalStorage',
      'Dialog',
      'BottomSheet',
    ],
  },

  // animations: 'all', // --- includes all animations
  // https://quasar.dev/options/animations
  animations: [
    'flipInX',
    'slideInDown',
  ],

  // https://v2.quasar.dev/quasar-cli-webpack/developing-ssr/configuring-ssr
  ssr: {
    pwa: false,

    // manualStoreHydration: true,
    // manualPostHydrationTrigger: true,

    prodPort: 3000, // The default port that the production server should use
    // (gets superseded if process.env.PORT is specified at runtime)

    maxAge: 1000 * 60 * 60 * 24 * 30,
    // Tell browser when a file from the server should expire from cache (in ms)

    // chainWebpackWebserver (/* chain */) {},

    middlewares: [
      ctx.prod ? 'compression' : '',
      'render', // keep this as last one
    ],
  },

  // https://v2.quasar.dev/quasar-cli-webpack/developing-pwa/configuring-pwa
  pwa: {
    workboxPluginMode: 'GenerateSW', // 'GenerateSW' or 'InjectManifest'
    workboxOptions: {}, // only for GenerateSW

    // for the custom service worker ONLY (/src-pwa/custom-service-worker.[js|ts])
    // if using workbox in InjectManifest mode
    // chainWebpackCustomSW (/* chain */) {},

    manifest: {
      name: 'Recalbox Manager v3',
      short_name: 'Recalbox Manager v3',
      description: '',
      display: 'standalone',
      orientation: 'portrait',
      background_color: '#ffffff',
      theme_color: '#027be3',
      icons: [
        {
          src: 'icons/icon-128x128.png',
          sizes: '128x128',
          type: 'image/png',
        },
        {
          src: 'icons/icon-192x192.png',
          sizes: '192x192',
          type: 'image/png',
        },
        {
          src: 'icons/icon-256x256.png',
          sizes: '256x256',
          type: 'image/png',
        },
        {
          src: 'icons/icon-384x384.png',
          sizes: '384x384',
          type: 'image/png',
        },
        {
          src: 'icons/icon-512x512.png',
          sizes: '512x512',
          type: 'image/png',
        },
      ],
    },
  },

  // Full list of options: https://v2.quasar.dev/quasar-cli-webpack/developing-cordova-apps/configuring-cordova
  cordova: {
    // noIosLegacyBuildFlag: true, // uncomment only if you know what you are doing
  },

  // Full list of options: https://v2.quasar.dev/quasar-cli-webpack/developing-capacitor-apps/configuring-capacitor
  capacitor: {
    hideSplashscreen: true,
  },

  // Full list of options: https://v2.quasar.dev/quasar-cli-webpack/developing-electron-apps/configuring-electron
  electron: {
    bundler: 'packager', // 'packager' or 'builder'

    packager: {
      // https://github.com/electron-userland/electron-packager/blob/master/docs/api.md#options

      // OS X / Mac App Store
      // appBundleId: '',
      // appCategoryType: '',
      // osxSign: '',
      // protocol: 'myapp://path',

      // Windows only
      // win32metadata: { ... }
    },

    builder: {
      // https://www.electron.build/configuration/configuration

      appId: 'recalbox-manager-v3-webpack',
    },

    // "chain" is a webpack-chain object https://github.com/neutrinojs/webpack-chain
    chainWebpackMain(/* chain */) {
      // do something with the Electron main process Webpack cfg
      // extendWebpackMain also available besides this chainWebpackMain
    },

    // "chain" is a webpack-chain object https://github.com/neutrinojs/webpack-chain
    chainWebpackPreload(/* chain */) {
      // do something with the Electron main process Webpack cfg
      // extendWebpackPreload also available besides this chainWebpackPreload
    },
  },
}));
