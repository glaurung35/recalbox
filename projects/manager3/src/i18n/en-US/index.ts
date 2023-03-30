/**
 * @author Nicolas TESSIER aka Asthonishia
 */
export default {
  404: {
    message: 'Sorry, nothing here...',
    returnButton: 'Go back',
  },
  components: {
    wrappedSelect: {
      noResults: 'No options',
    },
  },
  general: {
    virtualDevices: {
      tooltip: 'Access virtual devices',
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
      internationalization: {
        title: 'Internationalization',
        langSelect: {
          label: 'System language',
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
          help: 'Available options (in seconds) :'
            + '<ul><li><b>0 :</b> Video will be stopped when emulationstation is ready to start,</li>'
            + '<li><b>-1 :</b> All the video will be played before emulationstation start (default),</li>'
            + '<li><b>> 0 :</b> Time the video will be played before emulationstation start.</li></ul>',
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
      services: {
        title: 'Services',
        managerEnabled: 'Enables manager',
        sambaEnabled: 'Enables Samba',
        virtualGamepadsEnabled: 'Enables virtual devices',
        sshEnabled: 'Enables SSH protocol',
      },
      demo: {
        title: 'Demo screensaver',
        systemlist: {
          label: 'Displayed systems list',
          help: 'Set the system list from which ES will run random games.'
            + ' Empty list or unexisting key means all available systems.',
        },
        duration: {
          label: 'Demo duration ( 30 à 600 s )',
          help: 'Default demo game sessions last 90s.\n\nChange this value if you want shorter or longer sessions.',
        },
        infoscreenduration: {
          label: 'Info screen duration ( 15 à 30 s )',
          help: 'Default game info screen duration lasts 6s.\n\nChange this value if you want shorter or longer info screens.',
        },
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
      display: {
        title: 'Display',
        videomode: {
          label: 'Video mode',
          help: 'There are two groups of video modes: CEA and DMT. Make sure the mode is compatible with your native screen resolution.',
        },
      },
      menus: {
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
        startFromFirstSystem: {
          label: 'Start on the list of games from the first system (or the one selected)',
          help: 'ES will launch and display the list of games from the first system (or the one selected).',
        },
        disableSystemSelection: {
          label: 'Disable the system selection',
          help: 'ES will launch and display ONLY the first system (or the one selected).',
        },
        showOnlyScrapedGames: {
          label: 'Show only scraped games',
          help: 'ES will only display the games included in the gamelist.xml files (accelerates display times).',
        },
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
      netplay: 'netplay',
      retroachievements: 'retroachievements',
      bios: 'bios',
      systems: 'systems / roms',
      controllers: 'controllers',
      screenshots: 'screenshots',
    },
    global: {
      display: {
        title: 'Display',
        ratio: {
          label: 'Screen ratio',
          options: {
            automatic: 'Automatic',
            custom: 'Custom',
          },
        },
        videomode: {
          label: 'Video mode',
          help: 'There are two groups of video modes: CEA and DMT. Make sure the mode is compatible with your native screen resolution.',
        },
        shaders: {
          label: 'Activer les shaders',
        },
        shaderset: {
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
      },
      rewind: {
        title: 'Rewind',
        label: 'Enables rewind',
        help: 'The rewind option allows you to rewind the game whilst playing. This can slowdown some emulators (snes, psx) if you enable it.',
      },
      autosave: {
        title: 'Auto save',
        label: 'Enables automatic backup / load',
        help: 'This option allows you to create an automatic backup of your game when you exit it, then to load it again when you restart it.\n\n'
          + 'Once the game is launched and the backup loaded, if you want to return to the title screen of the game, use the special reset command.',
      },
      translation: {
        title: 'Retroarch translation',
        translate: {
          label: 'Enables automatic Retroarch translation',
        },
        translateFrom: {
          label: 'Source language',
          help: 'Setting the source language may speed up or give more accurate results.',
        },
        translateTo: {
          label: 'Target language',
          help: 'The default value is extracted from system language or, if system language is undefined, set to auto (EN).',
        },
        translateApiKey: {
          label: 'zTranslate API Key',
          help: 'Go to https://ztranslate.net and create an account.\n\nvalidate your account, then log in and go to the settings page and get the API Key at the bottom.',
        },
        translateUrl: {
          label: 'Other translation service URL',
          help: 'If you want to use another translation service or a custom API call, use this key to specify the url to call.\n\nIf the key is not empty, it is used instead of zTranslation\'s API Key.',
        },
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
      arcade: {
        title: 'Arcade',
        activate: {
          label: 'Enables "Arcade" meta system',
          help: 'Activate the Arcade metasystem to group all games from piFBA, FBN (libretro), MAME and optionally Neogeo into a single "Arcade" system.',
        },
        position: {
          label: '"Arcade" system position',
          help: 'You may want to specify its position in the system list. Default position is 0, negatives values may be used to start from the end.',
        },
        includeNeogeo: {
          label: 'Include Neogeo',
          help: 'Include NeoGeo or not in the meta system "Arcade".',
        },
        hideOriginals: {
          label: 'Hide originals systems',
          help: 'Hide included system or leave them in the system list.',
        },
      },
      netplay: {
        title: 'Netplay',
        activate: {
          label: 'Enables netplay',
          help: 'Netplay is a feature allowing networked multiplayer mode with some emulators via peer-to-peer (P2P) networking.',
        },
        nickname: {
          label: 'Nickname',
          help: 'Configure your nickname, be careful not to put an accent, this prevents the operation of Netplay.',
        },
        port: {
          label: 'Port',
          help: '55435 (Corresponds to the RetroArch server)',
        },
        relay: {
          label: 'Netplay MITM Server relay',
          help: 'The Netplay MITM, Relay Server option allows you to override the opening of your modem ports.\n'
            + 'This allows both sides of the connection to be routed through an intermediary server.\n\n'
            + 'However this will have an impact on the speed at which the data synchronizes and therefore on the responsiveness of the game..',
        },
        lobby: {
          label: 'Libretro lobby',
          help: 'URL of the libretro lobby list to use.\n\nThe URL "http://lobby.libretro.com/list/" is configured by default.',
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
