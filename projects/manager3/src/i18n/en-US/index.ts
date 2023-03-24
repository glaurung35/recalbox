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
    helpButton: {
      tooltip: 'Help',
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
      updateSuccess: 'Update completed',
      updateError: 'Error during update :',
      networkError: 'Network error',
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
    cpuVsMemory: {
      temperature: {
        title: 'CPU °C ( LAST 30S )',
      },
      memory: {
        title: 'USED MEMORY % ( LAST 30S )',
      },
    },
    cores: {
      title: 'CPU CORES USAGE',
      used: 'Used',
      free: 'Available',
    },
  },
  settings: {
    menu: {
      system: 'system',
      audio: 'audio',
      network: 'network',
      virtual_devices: 'Virtual devices',
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
      splashScreen: {
        title: 'Splash screen',
        splashLength: {
          label: 'Splash screen duration',
        },
      },
      specialKey: {
        title: 'Hot key',
        help: 'Available options :'
          + '<ul><li><b>default</b> is the default behaviour with all options,</li>'
          + '<li><b>nomenu</b> disables the menu,</li>'
          + '<li><b>none</b> disables the hotkey.</li></ul>',
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
        help: 'You need to restart EmulationStation to activate this feature',
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
        region: {
          title: 'Wifi country code',
          help: 'The default value is JP because this country is the most permissive for wifi.',
        },
        ip: {
          title: 'Recalbox IP',
        },
        netmask: {
          title: 'Netmask',
        },
        gateway: {
          title: 'Gateway IP',
        },
        toggleButtonLabel: 'Enable Wi-Fi',
        help: 'Please note: any misconfiguration would lead to your recalbox being unavailable on the network.\n\n'
          + 'The passwords are encrypted and are not re-displayed. This does not mean that the fields are emtpy! If you enter a new value, it will overwrite the old one.',
        ssid: 'SSID',
        key: 'Key',
      },
    },
    emustation: {
      options: {
        title: 'Settings',
        menuStyle: {
          label: 'Menu style',
          help: 'Available options :'
          + '<ul><li><b>bartop</b> has filtered options, suitable for bartops,</li>'
          + '<li><b>default</b> is the fully enabled menu,</li>'
          + '<li><b>none</b> just lets you select games.</li></ul>',
        },
        systemSelectedAsDefault: {
          label: 'System selected as default',
        },
        help_2: 'ES will launch and display the list of games from the first system (or the one selected).',
        startFromFirstSystem: 'Start on the list of games from the first system (or the one selected)',
        help_3: 'ES will launch and display ONLY the first system (or the one selected).',
        disableSystemSelection: 'Disable the system selection',
        help_4: 'ES will only display the games included in the gamelist.xml files (accelerates display times).',
        showOnlyScrapedGames: 'Show only scraped games',
      },
      videoSnaps: {
        title: 'Videosnaps',
        delay: {
          title: 'Delay',
          help: 'Delay before Videosnaps start, in millisecond. Default: 2s ( 2000 )',
        },
        loop: {
          title: 'Video loop times',
          help: '0 = no video. 1 or more = the video loops x times before fading out',
        },
      },
    },
    scraper: {
      settings: {
        title: 'Settings',
        getNameFrom: {
          title: 'Get names from',
          help: 'Available options :'
          + '<ul><li><b>0</b> from scrapper result (default),</li>'
          + '<li><b>1</b> from filename,</li>'
          + '<li><b>2</b> from filename undecorated (i.e: (text) and [text] removed ).</li></ul>',
        },
      },
      screenscraper: {
        medias: {
          title: 'ScreenScraper medias',
          media: 'Picture type',
          thumbnail: 'Thumbnail type',
          video: 'Video type',
          maps: 'Maps download',
          manual: 'Manuals download',
        },
        auth: {
          title: 'ScreenScraper authentication',
          user: 'User',
          password: 'Password',
        },
        i18n: {
          title: 'ScreenScraper international',
          language: 'Favorite language',
          region: 'Favorite region',
        },
        p2k: 'PAD to keyboard configurations download',
      },
    },
    kodi: {
      status: {
        title: 'Status',
        enableKodi: 'Enable Kodi',
        kodiAtStartup: 'Launch Kodi at startup',
        launchWithX: 'Key X starts Kodi',
      },
      video: {
        title: 'Video',
        select: {
          options: {
            label: 'Select mode',
          },
        },
      },
      network: {
        title: 'Network',
        waitMode: {
          title: 'Wait mode',
          help: 'Available options :'
            + '<ul><li><b>required</b> kodi does not launch before being able to reach the specified ip address,</li>'
            + '<li><b>wish</b> uses a countdown timer before starting,</li>'
            + '<li><b>nonce</b> no value.</li></ul>',
          select: {
            options: {
              label: 'Select mode',
            },
          },
        },
        waitTime: {
          title: 'Wait time',
          help: 'Waiting time (seconds) before kodi startup fails '
            + '("required") or continue ("wish")',
        },
        waitHost: 'IP Address',
      },
    },
    hyperion: {
      settings: {
        title: 'Settings',
        enableHyperion: 'Enable Hyperion',
      },
    },
  },
  emulation: {
    menu: {
      global: 'global',
      bios: 'bios',
      systems: 'systems / roms',
      controllers: 'controllers',
      screenshots: 'screenshots',
    },
    global: {
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
        help: 'Available shaders:'
          + '<ul><li><b>custom</b> allows you to choose and activate a custom shader,</li>'
          + '<li><b>none</b> don\'t enable shaders,</li>'
          + '<li><b>retro</b> set the "best" shader for each system, as chosen by '
          + 'the community. This will give you the gaming experience closest to the original system,</li>'
          + '<li><b>scanlines</b> enable scanlines for all emulators.</li></ul>',
      },
      customshaders: {
        label: 'Custom shader',
      },
      integerscale: {
        title: 'Integer scale',
        label: 'Enables pixel perfect',
        help: 'Activate the "Pixel Perfect" image scaling mode which is to get the exact rendering of the games on a modern screen.',
      },
      smooth: {
        title: 'Smooth game',
        label: 'Enables game smoothing',
      },
      rewind: {
        title: 'Rewind',
        label: 'Enables rewind',
        warning: 'The rewind option allows you to rewind the game whilst playing. This can slowdown some emulators (snes, psx) if you enable it.',
      },
      autosave: {
        title: 'Auto save',
        label: 'Enables automatic backup / load',
        help: 'This option allows you to create an automatic backup of your game when you exit it, then to load it again when you restart it.\n\n'
          + 'Once the game is launched and the backup loaded, if you want to return to the title screen of the game, use the special reset command.',
      },
      retroachievements: {
        title: 'Retroachievements',
        activate: {
          label: 'Enable RetroAchievements',
          help: 'RetroAchievements.org is a community website that allows you to win custom-made achievements for retro games.\n\n'
          + 'Achievements are made by and for the community.',
        },
        hardcore: {
          label: 'Enable Hardcore mode',
          help: 'The Hardcore mode deactivates all possibilities of saving in the emulator! You won\'t be able to save or reload your game whilst playing.\n\n'
          + 'You have to complete the game and get the achievements first time, just like it was on the original console.',
        },
        username: {
          label: 'Login',
        },
        password: {
          label: 'Password',
        },
      },
    },
    bios: {
      loader: 'Checking BIOS on server...',
      dialogs: {
        remove: {
          text: 'Are you sure you want to delete this BIOS ?',
        },
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
          label: 'Filter BIOS not present or wrong MD5',
        },
        orangeFilter: {
          label: 'Filter optional or non-validated BIOS',
        },
        greenFilter: {
          label: 'Filter valid BIOS',
        },
      },
      help: {
        tooltip: 'All about color codes',
        title: 'BIOS Status',
        green: {
          title: 'BIOS present + valid MD5',
        },
        yellow_1: {
          title: 'BIOS present + MD5 invalid but MD5 match not required',
          text: 'It may work, but since no one from us has tested it, we can\'t guarantee anything.',
        },
        yellow_2: {
          title: 'BIOS absent but optional',
          text: 'It can work, as long as you don\'t use a game or a feature that requires this optional bios',
        },
        red: {
          title: 'BIOS missing or MD5 invalid',
        },
      },
    },
    roms: {
      dialogs: {
        remove: {
          text: 'Are you sure you want to delete this ROM ?',
        },
      },
    },
    controllers: {
      bluetooth: {
        title: 'Bluetooth',
        enabled: 'Bluetooth controllers support',
        ertm: 'ERTM protocol activation',
      },
      db9: {
        title: 'DB9',
        enabled: 'DB9 controllers support',
        settings: 'Settings',
      },
      gamecon: {
        title: 'Gamecon',
        enabled: 'Gamecon controllers support',
        settings: 'Settings',
      },
      gpio: {
        title: 'Gpio',
        enabled: 'Gpio controllers support',
        settings: 'Settings',
      },
      ps3: {
        title: 'PS3',
        enabled: 'PS3 controllers support',
        label: 'Driver to use',
      },
      steam: {
        title: 'Steam',
        enabled: 'Steam controllers support',
      },
      xarcade: {
        title: 'XArcade',
        enabled: 'XArcade Tankstik and compatible controllers support',
      },
    },
    screenshots: {
      tooltip: 'Take a screenshot',
      dialogs: {
        remove: {
          text: 'Are you sure you want to delete this screenshot ?',
        },
      },
    },
  },
};
