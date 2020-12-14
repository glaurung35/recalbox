export default {
  404: {
    message: 'Sorry, nothing here...',
    returnButton: 'Go back',
  },
  general: {
    virtualDevices: {
      title: 'Virtual devices',
      keyboard: 'Keyboard',
      gamepad: 'Gamepad',
      trackpad: 'Trackpad',
    },
    confirmDialog: {
      cancel: 'Cancel',
      valid: 'Validate',
      defaultMessage: 'Are you sure you want to do that ?',
    },
    return: 'back',
    tables: {
      searchLabel: 'Search',
      noDataMessage: 'Well this is sad...',
    },
    copyToClipboard: 'Copied to clipboard',
    notify: {
      updateSuccess: 'Update successfully completed',
      updateError: 'Error during update :',
    },
    upload: {
      clearAll: 'Clear All',
      removeUploaded: 'Remove Uploaded Files',
      pickFiles: 'Pick files',
      minimize: 'Minimize',
      maximize: 'Maximize',
      close: 'Close',
      abortUpload: 'Abort upload',
      status: 'Status :',
    },
  },
  i18nMenu: {
    english: 'English',
    french: 'French',
  },
  menu: {
    home: 'Home',
    monitoring: 'Monitoring',
    settings: 'Settings',
    emulation: 'Emulation',
  },
  home: {
    preview: {
      title: 'Preview',
      systems: 'systems',
      roms: 'roms',
      sharePercent: 'share: partition %',
      adefinir: 'À définir !',
    },
  },
  monitoring: {
    temperature: {

    },
    cores: {
      used: 'Used',
      free: 'Available',
    }
  },
  settings: {
    menu: {
      system: 'system',
      audio: 'audio',
      network: 'network',
      virtual_devices: 'Virtual devices'
    },
    system: {
      settings: {
        title: 'settings',
        langSelect: {
          label: 'System languages',
        },
        keyboardSelect: {
          label: 'Keyboard layout',
          options: {},
        },
        timeZoneSelect: {
          label: 'Timezone',
          options: {},
        },
      },
      specialKey: {
        title: 'special key',
        paragraph_1: 'Behaviour of the hotkey',
        paragraph_2: 'Available options :' +
          '<ul><li><b>default</b> is the default behaviour with all options,</li>' +
          '<li><b>nomenu</b> disables the menu,</li>' +
          '<li><b>none</b> disables the hotkey.</li></ul>',
        behaviourSelect: {
          label: 'Behaviour',
          options: {},
        },
      },
      updates: {
        title: 'updates',
        toggleButtonLabel: 'Check for updates at startup',
        versionType: {
          label: 'Branch',
          options: {},
        },
      },
      versions: {
        title: 'Versions',
      },
    },
    audio: {
      backgroundMusic: {
        title: 'Background music',
        paragraph_1: 'You need to restart EmulationStation to activate this feature',
        toggleButtonLabel: 'Activate background music',
      },
      volume: {
        title: 'Volume',
      },
      audioOutput: {
        title: 'Audio output',
        select: {
          options: {
            label: 'Select audio output',
          },
        },
      },
    },
    network: {
      server: {
        title: 'Server',
        hostname: 'Hostname',
      },
      wifi: {
        title: 'Wi-Fi',
        toggleButtonLabel: 'Enable Wi-Fi',
        paragraphe_1: 'Please note: any misconfiguration would lead to your recalbox being unavailable on the network.\n' +
          'The passwords are encrypted and are not re-displayed. This does not mean that the fields are emtpy! If you enter a new value, it will overwrite the old one.',
        wifi_1: 'Wifi 1',
        wifi_2: 'Wifi 2',
        wifi_3: 'Wifi 3',
        ssid: 'SSID',
        key: 'Key',
      },
    },
    emustation: {
      options: {
        title: 'Options',
        paragraph_1: 'Available options :' +
          '<ul><li><b>bartop</b> has filtered options, suitable for bartops,</li>' +
          '<li><b>default</b> is the fully enabled menu,</li>' +
          '<li><b>none</b> just lets you select games.</li></ul>',
        menuStyle: {
          label: 'Menu style',
        },
        systemSelectedAsDefault: {
          label: 'System selected as default',
        },
        paragraph_2: 'ES will launch and display the list of games from the first system (or the one selected).',
        startFromFirstSystem: 'Start on the list of games from the first system (or the one selected)',
        paragraph_3: 'ES will launch and display ONLY the first system (or the one selected).',
        disableSystemSelection: 'Disable the system selection',
        paragraph_4: 'ES will only display the games included in the gamelist.xml files (accelerates display times).',
        showOnlyScrapedGames: 'Show only scraped games',
      },
    },
    kodi: {
      options: {
        title: 'Options',
        enableKodi: 'Enable Kodi',
        kodiAtStartup: 'Launch Kodi at startup',
        launchWithX: 'Key X starts Kodi',
      },
    },
  },
  emulation: {
    menu: {
      global: 'global',
      bios: 'bios',
      systemsRoms: 'systems / roms',
      controllers: 'controllers',
      screenshots: 'screenshots',
    },
    systems: {
      ratio: {
        title: 'Screen ratio',
        label: 'Ratio',
        options: {
          automatic: 'Automatic',
          custom: 'Custom',
        },
      },
      shaders: {
        title: 'Shaders set',
        label: 'Shaders set',
        paragraph: 'Available shaders:' +
          '<ul><li><b>custom</b> allows you to choose and activate a custom shader,</li>' +
          '<li><b>none</b> don\'t enable shaders,</li>' +
          '<li><b>retro</b> set the "best" shader for each system, as chosen by ' +
          'the community. This will give you the gaming experience closest to the original system,</li>' +
          '<li><b>scanlines</b> enable scanlines for all emulators.</li></ul>',
      },
      customshaders: {
        label: 'Custom shader',
      },
      integerscale: {
        title: 'Antialiasing',
        label: 'Filter to soften dilated pixels by scaling.',
      },
      rewind: {
        title: 'Rewind',
        label: 'The rewind option allows you to rewind the game whilst playing.',
        warning: 'This can slowdown some emulators (snes, psx) if you enable it.',
      },
      autosave: {
        title: 'Auto save / load',
        label: 'Enables automatic backup',
        paragraph: 'This option allows you to create an automatic backup of your game when you exit it, then to load it again when you restart it.\n' +
          'Once the game is launched and the backup loaded, if you want to return to the title screen of the game, use the special reset command.',
      },
      retroachievements: {
        title: 'Retroachievements',
        label: 'Retroachievements',
        activate: 'Enable RetroAchievements',
        hardcore: 'Enable Hardcore mode',
        username: 'Login',
        password: 'Password',
        paragraph_1: 'RetroAchievements.org ( <a href="http://retroachievements.org/" target="_blank" title="RetroAchievements.org">http://retroachievements.org/</a> ) is a community website that allows you to win custom-made achievements for retro games.',
        paragraph_2: 'Achievements are made by and for the community.',
        paragraph_3: 'The Hardcore mode deactivates all possibilities of saving in the emulator! You won\'t be able to save or reload your game whilst playing.\n' +
          'You have to complete the game and get the achievements first time, just like it was on the original console.',
      },
    },
    bios: {
      loader: 'Checking BIOS on server...',
      dialogs: {
        remove: {
          text: 'Are you sure you want to delete this BIOS ?'
        }
      },
      tableHeader: {
        system: 'System',
        bios: 'BIOS',
        currentMd5: 'Current MD5',
        md5: 'Expected MD5\'s',
        valid: 'Valid',
      },
      tooltips: {
        green: 'Verified BIOS',
        yellow: 'BIOS optional or not verified',
        red: 'BIOS not present or wrong MD5',
        delete: 'Delete BIOS',
        redFilter: {
          label: 'Filter BIOS not present or wrong MD5'
        },
        orangeFilter: {
          label: 'Filter optional or non-validated BIOS'
        },
        greenFilter: {
          label: 'Filter valid BIOS'
        }
      },
      help: {
        tooltip:'All about color codes',
        title: 'BIOS Status',
      },
    },
    roms: {
      dialogs: {
        remove: {
          text: 'Are you sure you want to delete this ROM ?'
        }
      },
    },
    controllers: {
      db9: {
        title: 'DB9 controller',
        activate: 'DB9 controllers support',
        settings: 'Settings',
      },
      gamecon: {
        title: 'Gamecon controller',
        activate: 'Gamecon controllers support',
        settings: 'Settings',
      },
      gpio: {
        title: 'Gpio controller',
        activate: 'Gpio controllers support',
        settings: 'Settings',
      },
      ps3: {
        title: 'PS3 controller',
        activate: 'PS3 controllers support',
        label: 'Driver to use',
      },
    },
    screenshots: {
      screenshotButton: 'take a screenshot',
      dialogs: {
        remove: {
          text: 'Are you sure you want to delete this screenshot ?'
        }
      },
    },
  },
}
