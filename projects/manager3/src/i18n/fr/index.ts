export default {
  404: {
    message: 'Désolé, rien ici...',
    returnButton: 'Retour',
  },
  components: {
    wrappedSelect: {
      noResults: 'Aucune option',
    },
  },
  general: {
    virtualDevices: {
      tooltip: 'Accéder aux périphériques virtuels',
      title: 'Périphériques virtuels',
      keyboard: 'Clavier',
      gamepad: 'Manette',
      trackpad: 'Pavet tactile',
    },
    helpButton: {
      tooltip: 'Aide',
    },
    confirmDialog: {
      cancel: 'Annuler',
      valid: 'Valider',
      defaultMessage: 'Êtes-vous certain de vouloir faire ça ?',
    },
    return: 'retour',
    tables: {
      searchLabel: 'Recherche',
      noDataMessage: 'Et bien c\'est triste...',
    },
    copyToClipboard: 'Copié dans le presse-papiers',
    notify: {
      updateSuccess: 'Modification effectuée',
      updateError: 'Erreur lors de la modification : ',
      networkError: 'Problème de connexion réseau',
    },
    upload: {
      clearAll: 'Effacer tout',
      removeUploaded: 'Supprimer les fichiers chargés',
      pickFiles: 'Sélectionner des fichiers',
      minimize: 'Minimiser',
      maximize: 'Maximiser',
      close: 'Fermer',
      abortUpload: 'Annuler l\'envoi',
      status: 'Statut :',
    },
  },
  i18nMenu: {
    english: 'Anglais',
    french: 'Français',
  },
  menu: {
    home: 'Accueil',
    monitoring: 'Monitoring',
    settings: 'Configuration',
    emulation: 'Émulation',
  },
  home: {
    preview: {
      title: 'Aperçu',
      systems: 'systèmes',
      roms: 'roms',
      sharePercent: 'share: % de la partition',
    },
  },
  monitoring: {
    cpuVsMemory: {
      temperature: {
        title: 'CPU °C ( DERNIÈRES 30S )',
      },
      memory: {
        title: 'MÉMOIRE UTILISÉE % ( DERNIÈRES 30S )',
      },
    },
    cores: {
      title: 'UTILISATION DES CORE CPU',
      used: 'Utilisé',
      free: 'Disponible',
    },
  },
  settings: {
    menu: {
      system: 'système',
      audio: 'audio',
      network: 'réseau',
      virtual_devices: 'Périphériques virtuels',
    },
    system: {
      internationalization: {
        title: 'Internationalisation',
        langSelect: {
          label: 'Langues du système',
        },
        keyboardSelect: {
          label: 'Disposition du clavier',
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
          label: 'Durée du splash screen',
          help: 'Options disponibles (en secondes) :'
            + '<ul><li><b>0 :</b> La vidéo s\'arrêtera lorsqu\'EmulationStation sera prêt à démarrer,</li>'
            + '<li><b>-1 :</b> Toute la vidéo sera lue avant le démarrage d\'EmulationStation (par défaut),</li>'
            + '<li><b>> 0 :</b> Temps de lecture de la vidéo avant le démarrage d\'EmulationStation.</li></ul>',
        },
      },
      specialKey: {
        title: 'hot key',
        help: 'Options disponibles :'
          + '<ul><li><b>default</b> correspond au fonctionnement de base avec toutes les possibilités,</li>'
          + '<li><b>nomenu</b> désactive l\'affichage du menu,</li>'
          + '<li><b>none</b> désactive la touche spéciale.</li></ul>',
        behaviourSelect: {
          label: 'Comportement',
          options: {},
        },
      },
      updates: {
        title: 'mises à jour',
        toggleButtonLabel: 'Vérifier les mises à jours au démarrage du système',
        versionType: {
          label: 'Type de version',
          options: {},
        },
      },
      services: {
        title: 'Services',
        managerEnabled: 'Activer le manager',
        sambaEnabled: 'Activer Samba',
        virtualGamepadsEnabled: 'Activer les périphériques virtuels',
        sshEnabled: 'Activer le protocole SSH',
      },
      demo: {
        title: 'Démo screensaver',
        systemlist: {
          label: 'Liste des systèmes présentés',
          help: 'Configure la liste des systèmes parmis lesquels EmulationStation va puiser pour faire tourner les démo.'
            + ' Si une valeur nulle ou inconnue est saisie, toute les systèmes seront présentés.',
        },
        duration: {
          label: 'Durée de la démo de jeux ( 30 à 600 s )',
          help: 'Valeur par défaut : 90s.\n\nChangez cette valeur si vous souhaitez une durée plus courte ou plus longue.',
        },
        infoscreenduration: {
          label: 'Durée des écrans d\'information ( 15 à 30 s )',
          help: 'Valeur par défaut : 6s.\n\nChangez cette valeur si vous souhaitez une durée plus courte ou plus longue.',
        },
      },
    },
    audio: {
      backgroundMusic: {
        title: 'Musique de fond',
        help: 'Cette modification nécessite de redémarrer EmulationStation pour être prise en compte',
        toggleButtonLabel: 'Activer les musiques de fond',
      },
      volume: {
        title: 'Volume',
      },
      audioOutput: {
        title: 'Sortie audio',
        select: {
          options: {
            label: 'Sélectionner la sortie audio',
          },
        },
      },
    },
    network: {
      server: {
        title: 'Serveur',
        hostname: 'Nom d\'hôte',
      },
      wifi: {
        title: 'Wi-Fi',
        region: {
          title: 'Indicatif pays du wifi',
          help: 'La valeur par défaut est JP car ce pays est le plus permissif en ce qui concerne le wifi.',
        },
        ip: {
          title: 'IP Recalbox',
        },
        netmask: {
          title: 'Masque de sous réseau',
        },
        gateway: {
          title: 'IP de la passerelle',
        },
        toggleButtonLabel: 'Activer le Wi-Fi',
        help: 'Attention, si vous vous trompez dans vos informations vous risquez de ne plus pouvoir accéder à votre recalbox par le wifi.\n\n'
          + 'Les clés sont chiffrées et ne sont donc pas réaffichées dans les champs. Cela ne veut pas dire que la valeur n\'est pas renseignée ! Si vous renseignez une valeur, cela écrasera la précédente.',
        ssid: 'SSID',
        key: 'Clé',
      },
    },
    emustation: {
      display: {
        title: 'Affichage',
        videomode: {
          label: 'Mode vidéo',
          help: 'Il existe deux groupes de modes vidéo : CEA et DMT. Assurez-vous que le mode est compatible avec votre résolution d\'écran native',
        },
      },
      menus: {
        title: 'Menus',
        menuStyle: {
          label: 'Style de menu',
          help: 'Options disponibles :'
          + '<ul><li><b>bartop</b> comporte peu de menu, seulement ceux nécessaires pour les bartops,</li>'
          + '<li><b>defaut</b> correspond au menu de base avec toutes les options,</li>'
          + '<li><b>none</b> n\'offre aucun menu excepté celui de recherche de jeux.</li></ul>',
        },
        systemSelectedAsDefault: {
          label: 'Système sélectionné par défaut',
        },
        startFromFirstSystem: {
          label: 'Démarrer sur la liste de jeux du premier système (ou de celui sélectionné)',
          help: 'ES se lancera et affichera directement la liste de jeux du premier système (ou celui sélectionné).',
        },
        disableSystemSelection: {
          label: 'Désactiver la sélection des systèmes',
          help: 'ES se lancera et n\'affichera QUE le premier système (ou celui sélectionné).',
        },
        showOnlyScrapedGames: {
          label: 'N\'afficher que les jeux parsés',
          help: 'ES n\'affichera que les jeux listés dans les fichiers gamelist.xml (accélère les temps d\'affichage).',
        },
      },
      videoSnaps: {
        title: 'Videosnaps',
        delay: {
          title: 'Délai',
          help: 'Délai avant le démarrage des Videosnaps, en millisecondes. Par défaut : 2s ( 2000 )',
        },
        loop: {
          title: 'Nombre de boucles vidéo',
          help: '0 = pas de vidéo. 1 ou plus = la vidéo boucle x fois avant de disparaître',
        },
      },
    },
    scraper: {
      settings: {
        title: 'Réglages',
        getNameFrom: {
          title: 'Obtenir les noms à partir de',
          help: 'Options disponibles :'
          + '<ul><li><b>0</b> à partir des résultats du scrap (défaut),</li>'
          + '<li><b>1</b> à partie du nom du fichier,</li>'
          + '<li><b>2</b> à partie du nom du fichier non décoré (i.e: (text) et [text] supprimés).</li></ul>',
        },
      },
      screenscraper: {
        medias: {
          title: 'ScreenScraper médias',
          media: 'Type d\'image',
          thumbnail: 'Type de vignette',
          video: 'Type de vidéo',
          maps: 'Téléchargement des cartes',
          manual: 'Téléchargement des manuels',
        },
        auth: {
          title: 'ScreenScraper authentification',
          user: 'Utilisateur',
          password: 'Mot de passe',
        },
        i18n: {
          title: 'ScreenScraper international',
          language: 'Langue préférée',
          region: 'Région préférée',
        },
        p2k: 'Téléchargement des configurations PAD vers clavier',
      },
    },
    kodi: {
      status: {
        title: 'Statut',
        enableKodi: 'Activer Kodi',
        kodiAtStartup: 'Lancer Kodi au démarrage',
        launchWithX: 'Lancer Kodi via la touche X',
      },
      video: {
        title: 'Vidéo',
        select: {
          options: {
            label: 'Sélectionner le mode de résolution',
          },
        },
      },
      network: {
        title: 'Réseau',
        waitMode: {
          title: 'Mode d\'attente',
          help: 'Options disponibles :'
            + '<ul><li><b>required</b> kodi ne se lance pas avant d\'avoir pu joindre l\'adresse ip indiquée,</li>'
            + '<li><b>wish</b> utilise un compte à rebours avant de démarrer,</li>'
            + '<li><b>nonce</b> aucune valeur.</li></ul>',
          select: {
            options: {
              label: 'Sélectionner le mode',
            },
          },
        },
        waitTime: {
          title: 'Temps d\'attente',
          help: 'Temps d\'attente (secondes) avant que le démarrage de kodi échoue '
            + '("required") ou continue ("wish")',
        },
        waitHost: 'Adresse IP',
      },
    },
    hyperion: {
      settings: {
        title: 'Réglages',
        enableHyperion: 'Activer Hyperion',
      },
    },
  },
  emulation: {
    menu: {
      global: 'global',
      netplay: 'netplay',
      retroachievements: 'retroachievements',
      bios: 'bios',
      systems: 'systèmes / roms',
      controllers: 'contrôleurs',
      screenshots: 'screenshots',
    },
    global: {
      display: {
        title: 'Affichage',
        ratio: {
          label: 'Ratio de l\'écran',
          options: {
            automatic: 'Automatique',
            custom: 'Personnalisé',
          },
        },
        videomode: {
          label: 'Mode vidéo',
          help: 'Il existe deux groupes de modes vidéo : CEA et DMT. Assurez-vous que le mode est compatible avec votre résolution d\'écran native.',
        },
        shaders: {
          label: 'Activer les shaders',
        },
        shaderset: {
          label: 'Set de shaders',
          help: 'Shaders disponibles:'
            + '<ul><li><b>custom</b> permet de choisir et d\'activer un shader custom,</li>'
            + '<li><b>none</b> ne met aucun shader.</li>'
            + '<li><b>retro</b> sélectionne le "meilleur" shader pour chaque système, choisi par la communauté.'
            + ' Il vous apportera l\'expérience de jeu la plus proche de l\'expérience originale,</li>'
            + '<li><b>scanlines</b> active les scanlines sur tous les emulateurs,</li></ul>',
        },

        customshaders: {
          label: 'Shader custom',
        },
        integerscale: {
          label: 'Activer le pixel perfect',
          help: 'Activez le mode de mise à l\'échelle de l\'image "Pixel Perfect" qui consiste à obtenir le rendu exact des jeux sur un écran moderne.',
        },
        smooth: {
          title: 'Lissage des jeux',
          label: 'Activer le lissage des jeux',
        },
      },
      rewind: {
        title: 'Rembobinage',
        label: 'Active le rembobinage des jeux',
        help: 'L\'option rembobinage vous autorise à effectuer des retours dans le temps lors de votre partie. Cela peut ralentir certains émulateurs (snes, psx) si vous l\'activez par défaut.',
      },
      autosave: {
        title: 'Sauvegarde',
        label: 'Active la sauvegarde / chargement automatique',
        help: 'Cette option vous permet de créer une sauvegarde automatique de votre jeu quand vous le quittez, puis de la charger à nouveau quand vous le relancerez.\n\n'
          + 'Une fois le jeu lancé et la sauvegarde chargée, si vous souhaitez revenir à l\'écran titre du jeu, utilisez la commande spéciale de reset.',
      },
      translation: {
        title: 'Traduction Retroarch',
        translate: {
          label: 'Activer la traduction automatique Retroarch',
        },
        translateFrom: {
          label: 'Langue source',
          help: 'Définir la langue source peut accélérer et/ou donner de meilleurs résultats',
        },
        translateTo: {
          label: 'Langue cible',
          help: 'La valeur par défaut est extraite depuis la langue système, ou définie automatiquement (EN) si la langue système est non définie.',
        },
        translateApiKey: {
          label: 'Clé API zTranslate',
          help: 'Allez sur https://ztranslate.net et créez vous un compte.\n\nValider votre compte, identifiez-vous puis récupérez la clé API en bas de page.',
        },
        translateUrl: {
          label: 'URL d\'un autre service de traduction',
          help: 'Si vous souhaitez utiliser un autre service de traduction ou un appel API custom, spécifiez l\'URL à appeler.\n\nSi cette clé est renseignée, elle est utilisée à l aplace du service zTranslate.',
        },
      },
      retroachievements: {
        title: 'Retroachievements',
        activate: {
          label: 'Activer RetroAchievements',
          help: 'RetroAchievements.org est un site communautaire qui permet de  gagner des hauts-faits sur mesure dans les jeux d\'arcade grâce à l\'émulation.\n\n'
          + ' Les haut-faits sont conçus par et pour la communauté.',
        },
        hardcore: {
          label: 'Activer le mode Hardcore',
          help: 'Le mode Hardcore désactive *toutes* les possibilités de sauvegarder dans l\'émulateur : vous ne pourrez ni sauvegarder ni recharger votre partie en cours de jeu.\n\n'
          + 'Vous devrez compléter le jeu et obtenir les hauts-faits du premier coup, comme c\'était le cas sur la console originale !',
        },
        username: {
          label: 'Login',
        },
        password: {
          label: 'Mot de passe',
        },
      },
      arcade: {
        title: 'Arcade',
        activate: {
          label: 'Activer le meta système "Arcade"',
          help: 'Activer le meta système "Arcade" permet de grouper tous les jeux piFBA, FBN (libretro), MAME et Neogeo de manière optionnelle, dans un système unique "Arcade".',
        },
        position: {
          label: 'Position du système "Arcade"',
          help: 'Vous pouvez avoir envie de modifier la position du système dans la liste des systèmes.\n\nLa valeur par défaut est 0, les valeurs négatives peuvent être utilisées pour partir de la fin.',
        },
        includeNeogeo: {
          label: 'Inclure la Neogeo',
          help: 'Inclus la Neogeo dans la liste du meta système "Arcade"',
        },
        hideOriginals: {
          label: 'Masquer les systèmes originaux',
          help: 'Masque les systèmes inclus dans le meta système "Arcade".',
        },
      },
      netplay: {
        title: 'Netplay',
        activate: {
          label: 'Activer le netplay',
          help: 'Le Netplay est une fonctionnalité permettant le mode multijoueur en réseau avec certains émulateurs via la mise en réseau peer-to-peer (P2P).',
        },
        nickname: {
          label: 'Surnom',
          help: 'Configurez votre surnom, veillez à ne pas mettre d\'accent, cela empêche le fonctionnement du Netplay.',
        },
        port: {
          label: 'Port',
          help: '55435 (Correspond au serveur de RetroArch)',
        },
        relay: {
          label: 'Netplay MITM Server relais',
          help: 'L\'option Netplay MITM, Relay Server (Server relais) permet d’outrepasser l’ouverture des ports de votre modem.\n'
            + 'Cela permet d\'acheminer les deux côtés de la connexion via un serveur d\'intermédiaire.\n\n'
            + 'Cependant cela aura un impact sur la vitesse à laquelle les données se synchronisent et donc sur la réactivité du jeu.',
        },
        lobby: {
          label: 'Lobby libretro',
          help: 'URL de la liste de lobby libretro à utiliser.\n\nL\'URL "http://lobby.libretro.com/list/" est configurée par défaut.',
        },
      },
    },
    bios: {
      loader: 'Vérification des BIOS sur le serveur ...',
      dialogs: {
        remove: {
          text: 'Êtes-vous certain de vouloir supprimer ce BIOS ?',
        },
      },
      tableHeader: {
        system: 'Système',
        bios: 'BIOS',
        currentMd5: 'MD5 courant',
        md5: 'MD5 attendus',
        valid: 'Valide',
      },
      tooltips: {
        green: 'BIOS vérifié',
        yellow: 'BIOS facultatif ou non validé',
        red: 'BIOS non présent ou MD5 erroné',
        delete: 'Supprimer le BIOS',
        redFilter: {
          label: 'Filtrer les BIOS non présents ou MD5 erroné',
        },
        orangeFilter: {
          label: 'Filtrer les BIOS facultatifs ou non validés',
        },
        greenFilter: {
          label: 'Filtrer les BIOS valides',
        },
      },
      help: {
        tooltip: 'Tout savoir sur les codes couleur',
        title: 'Statuts des BIOS',
        green: {
          title: 'BIOS présent + MD5 valide',
        },
        yellow_1: {
          title: 'BIOS présent + MD5 invalide mais MD5 match non obligatoire',
          text: 'Ça peut fonctionner, mais comme personne de chez nous ne l\'a testé, on ne garantie rien',
        },
        yellow_2: {
          title: 'BIOS absent mais optionnel',
          text: 'Ça peut fonctionner, tant qu\'on utilise pas un jeu ou une fonctionnalité qui requiert ce bios optionnel',
        },
        red: {
          title: 'BIOS absent ou MD5 invalide',
        },
      },
    },
    roms: {
      dialogs: {
        remove: {
          text: 'Êtes-vous certain de vouloir supprimer cette ROM ?',
        },
      },
    },
    controllers: {
      bluetooth: {
        title: 'Bluetooth',
        enabled: 'Support des contrôleurs Bluetooth',
        ertm: 'Activation du protocole ERTM',
      },
      db9: {
        title: 'DB9',
        enabled: 'Support des contrôleurs DB9',
        settings: 'Paramètres',
      },
      gamecon: {
        title: 'Gamecon',
        enabled: 'Support des contrôleurs Gamecon',
        settings: 'Paramètres',
      },
      gpio: {
        title: 'Gpio',
        enabled: 'Support des contrôleurs Gpio',
        settings: 'Paramètres',
      },
      ps3: {
        title: 'PS3',
        enabled: 'Support des contrôleurs PS3',
        label: 'Driver à utiliser',
      },
      steam: {
        title: 'Steam',
        enabled: 'Support des contrôleurs Steam',
      },
      xarcade: {
        title: 'XArcade',
        enabled: 'Support des contrôleurs XArcade Tankstik et contrôleurs compatibles',
      },
    },
    screenshots: {
      tooltip: 'Faire une capture',
      dialogs: {
        remove: {
          text: 'Êtes-vous certain de vouloir supprimer cette capture d\'écran ?',
        },
      },
    },
  },
};
