const routes = [
  {
    path: '',
    component: () => import('layouts/MainContainer.vue'),
    children: [
      {
        path: '/',
        name: 'home',
        component: () => import('pages/Home.vue'),
      }
    ]
  },
  {
    path: '',
    component: () => import('layouts/MainContainer.vue'),
    children: [
      {
        path: '/monitoring',
        name: 'monitoring',
        component: () => import('pages/Monitoring.vue'),
      }
    ]
  },
  {
    path: '',
    component: () => import('layouts/MainContainer.vue'),
    children: [
      {
        path: '/settings',
        name: 'settings',
        redirect: { name: 'system' },
        component: () => import('pages/Settings.vue'),
        children: [
          {
            path: 'system',
            name: 'system',
            component: () => import('components/settings/SettingsSystemTabContent.vue')
          },
          {
            path: 'audio',
            name: 'audio',
            component: () => import('components/settings/SettingsAudioTabContent.vue')
          },
          {
            path: 'network',
            name: 'network',
            component: () => import('components/settings/SettingsNetworkTabContent.vue')
          },
          {
            path: 'emustation',
            name: 'emustation',
            component: () => import('components/settings/SettingsEmuStationTabContent.vue')
          },
          {
            path: 'kodi',
            name: 'kodi',
            component: () => import('components/settings/SettingsKodiTabContent.vue')
          },
          {
            path: 'hyperion',
            name: 'hyperion',
            component: () => import('components/settings/SettingsHyperionTabContent.vue')
          }
        ]
      }
    ]
  },
  {
    path: '',
    component: () => import('layouts/MainContainer.vue'),
    children: [
      {
        path: '/emulation',
        name: 'emulation',
        redirect: { name: 'global' },
        component: () => import('pages/Emulation.vue'),
        children: [
          {
            path: 'global',
            name: 'global',
            component: () => import('components/emulation/EmulationGlobalTabContent.vue')
          },
          {
            path: 'bios',
            name: 'bios',
            component: () => import('components/emulation/EmulationBiosTabContent.vue')
          },
          {
            path: 'systems',
            name: 'systems',
            component: () => import('components/emulation/EmulationRomsTabSystems.vue')
          },
          {
            path: 'systems/:system',
            name: 'systems-system',
            component: () => import('components/emulation/EmulationRomsTabRoms.vue')
          },
          {
            path: 'systems/:system/:rom',
            name: 'systems-system-rom',
            component: () => import('components/emulation/EmulationRomsTabRoms.vue')
          },
          {
            path: 'controllers',
            name: 'controllers',
            component: () => import('components/emulation/EmulationControllersTabContent.vue')
          },
          {
            path: 'screenshots',
            name: 'screenshots',
            component: () => import('components/emulation/EmulationScreenshotsTabContent.vue')
          }
        ]
      }
    ]
  },
  {
    path: '',
    component: () => import('layouts/EmptyContainer.vue'),
    children: [
      {
        path: '/virtual-devices/keyboard',
        name: 'virtual-devices-keyboard',
        component: () => import('components/virtualdevices/Keyboard.vue')
      },
      {
        path: '/virtual-devices/gamepad',
        name: 'virtual-devices-gamepad',
        component: () => import('components/virtualdevices/Gamepad.vue')
      },
      {
        path: '/virtual-devices/trackpad',
        name: 'virtual-devices-trackpad',
        component: () => import('components/virtualdevices/Trackpad.vue')
      }
    ]
  }
]

// Always leave this as last one
if (process.env.MODE !== 'ssr') {
  routes.push({
    path: '*',
    component: () => import('pages/Error404.vue'),
  })
}

export default routes
