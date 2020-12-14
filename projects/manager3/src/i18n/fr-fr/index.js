export default {
  404: {
    message: 'Désolé, rien ici...',
    returnButton: 'Retour',
  },
  general: {
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
      updateSuccess: 'Modification effectuée avec succès',
      updateError: 'Erreur lors de la modification : ',
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
      adefinir: 'À définir !',
    },
  },
  monitoring: {
    temperature: {

    },
    cores: {
      used: 'Utilisé',
      free: 'Disponible',
    }
  },
  settings: {
    menu: {
      system: 'système',
      audio: 'audio',
      network: 'réseau',
      virtual_devices: 'Périphériques virtuels'
    },
    system: {
      settings: {
        title: 'réglages',
        langSelect: {
          label: 'Langues du système'
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
      specialKey: {
        title: 'touche spéciale',
        paragraph_1: 'Choix du comportement de la touche spéciale dans les émulateurs',
        paragraph_2: 'Options disponibles :' +
          '<ul><li><b>default</b> correspond au fonctionnement de base avec toutes les possibilités,</li>' +
          '<li><b>nomenu</b> désactive l\'affichage du menu,</li>' +
          '<li><b>none</b> désactive la touche spéciale.</li></ul>',
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
      versions: {
        title: 'Versions',
      },
    },
    audio: {
      backgroundMusic: {
        title: 'Musique de fond',
        paragraph_1: 'Cette modification nécessite de redémarrer EmulationStation pour être prise en compte',
        toggleButtonLabel: 'Activer les musiques de fond',
      },
      volume: {
        title: 'Volume',
      },
      audioOutput: {
        title: 'Sortie audio',
        select: {
          options: {
            label: 'Sélectionner la sortie audio'
          }
        }
      }
    },
    network: {
      server: {
        title: 'Serveur',
        hostname: 'Nom d\'hôte',
      },
      wifi: {
        title: 'Wi-Fi',
        toggleButtonLabel: 'Activer le Wi-Fi',
        paragraphe_1: 'Attention, si vous vous trompez dans vos informations vous risquez de ne plus pouvoir accéder à votre recalbox par le wifi.\n' +
          'Les clés sont cryptées et ne sont donc pas réaffichées dans les champs. Cela ne veut pas dire que la valeur n\'est pas renseignée ! Si vous renseignez une valeur, cela écrasera la précédente.',
        wifi_1: 'Wifi 1',
        wifi_2: 'Wifi 2',
        wifi_3: 'Wifi 3',
        ssid: 'SSID',
        key: 'Clé',
      },
    },
    emustation: {
      options: {
        title: 'Options',
        paragraph_1: 'Options disponibles :' +
          '<ul><li><b>bartop</b> comporte peu de menu, seulement ceux nécessaires pour les bartops,</li>' +
          '<li><b>defaut</b> correspond au menu de base avec toutes les options,</li>' +
          '<li><b>none</b> n\'offre aucun menu excepté celui de recherche de jeux.</li></ul>',
        menuStyle: {
          label: 'Style de menu'
        },
        systemSelectedAsDefault: {
          label: 'Système sélectionné par défaut'
        },
        paragraph_2: 'ES se lancera et affichera directement la liste de jeux du premier système (ou celui sélectionné).',
        startFromFirstSystem: 'Démarrer sur la liste de jeux du premier système (ou de celui sélectionné)',
        paragraph_3: 'ES se lancera et n\'affichera QUE le premier système (ou celui sélectionné).',
        disableSystemSelection: 'Désactiver la sélection des systèmes',
        paragraph_4: 'ES n\'affichera que les jeux listés dans les fichiers gamelist.xml (accélère les temps d\'affichage).',
        showOnlyScrapedGames: 'N\'afficher que les jeux parsés',
      },
    },
    kodi: {
      options: {
        title: 'Options',
        enableKodi: 'Activer Kodi',
        kodiAtStartup: 'Lancer Kodi au démarrage',
        launchWithX: 'Lancer Kodi via la touche X',
      },
    },
  },
  emulation: {
    menu: {
      global: 'global',
      bios: 'bios',
      systemsRoms: 'systèmes / roms',
      controllers: 'contrôleurs',
      screenshots: 'screenshots',
    },
    systems: {
      ratio: {
        title: 'Ratio de l\'écran',
        label: 'Ratio',
        options: {
          automatic: 'Automatique',
          custom: 'Personnalisé',
        },
      },
      shaders: {
        title: 'Set de shaders',
        label: 'Set de shaders',
        paragraph: 'Shaders disponibles:' +
          '<ul><li><b>custom</b> permet de choisir et d\'activer un shader custom,</li>' +
          '<li><b>none</b> ne met aucun shader.</li>' +
          '<li><b>retro</b> sélectionne le "meilleur" shader pour chaque système, choisi par la communauté.' +
          ' Il vous apportera l\'expérience de jeu la plus proche de l\'expérience originale,</li>' +
          '<li><b>scanlines</b> active les scanlines sur tous les emulateurs,</li></ul>',
      },
      customshaders: {
        label: 'Shader custom',
      },
      integerscale: {
        title: 'Lissage des jeux',
        label: 'Filtre permettant d\'adoucir les pixels dilatés par la mise à l\'échelle.',
      },
      rewind: {
        title: 'Rembobinage des jeux',
        label: 'L\'option rembobinage vous autorise à effectuer des retours dans le temps lors de votre partie.',
        warning: 'Cela peut ralentir certains émulateurs (snes, psx) si vous l\'activez par défaut.',
      },
      autosave: {
        title: 'Sauvegarde / Chargement automatique',
        label: 'Active la sauvegarde automatique',
        paragraph: 'Cette option vous permet de créer une sauvegarde automatique de votre jeu quand vous le quittez, puis de la charger à nouveau quand vous le relancerez.\n' +
          'Une fois le jeu lancé et la sauvegarde chargée, si vous souhaitez revenir à l\'écran titre du jeu, utilisez la commande spéciale de reset.',
      },
      retroachievements: {
        title: 'Retroachievements',
        label: 'Retroachievements',
        activate: 'Activer RetroAchievements',
        hardcore: 'Activer le mode Hardcore',
        username: 'Login',
        password: 'Mot de passe',
        paragraph_1: 'RetroAchievements.org ( <a href="http://retroachievements.org/" target="_blank" title="RetroAchievements.org">http://retroachievements.org/</a> ) est un site communautaire qui permet de  gagner des hauts-faits sur mesure dans les jeux d\'arcade grâce à l\'émulation.',
        paragraph_2: 'Les haut-faits sont conçus par et pour la communauté.',
        paragraph_3: 'Le mode Hardcore désactive *toutes* les possibilités de sauvegarder dans l\'émulateur : vous ne pourrez ni sauvegarder ni recharger votre partie en cours de jeu.\n' +
          'Vous devrez compléter le jeu et obtenir les hauts-faits du premier coup, comme c\'était le cas sur la console originale !',
      }
    },
    bios: {
      loader: 'Vérification des BIOS sur le serveur ...',
      dialogs: {
        remove: {
          text: 'Êtes-vous certain de vouloir supprimer ce BIOS ?'
        }
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
          label: 'Filtrer les BIOS non présents ou MD5 erroné'
        },
        orangeFilter: {
          label: 'Filtrer les BIOS facultatifs ou non validés'
        },
        greenFilter: {
          label: 'Filtrer les BIOS valides'
        }
      },
      help: {
        tooltip:'Tout savoir sur les codes couleur',
        title: 'Statuts des BIOS',
      },
    },
    roms: {
      dialogs: {
        remove: {
          text: 'Êtes-vous certain de vouloir supprimer cette ROM ?'
        }
      },
    },
    controllers: {
      db9: {
        title: 'Contrôleur DB9',
        activate: 'Support des contrôleurs DB9',
        settings: 'Paramètres',
      },
      gamecon: {
        title: 'Contrôleur Gamecon',
        activate: 'Support des contrôleurs Gamecon',
        settings: 'Paramètres',
      },
      gpio: {
        title: 'Contrôleur Gpio',
        activate: 'Support des contrôleurs Gpio',
        settings: 'Paramètres',
      },
      ps3: {
        title: 'Contrôleur PS3',
        activate: 'Support des contrôleurs PS3',
        label: 'Driver à utiliser',
      },
    },
    screenshots: {
      screenshotButton: 'faire une capture',
      dialogs: {
        remove: {
          text: 'Êtes-vous certain de vouloir supprimer cette capture d\'écran ?'
        }
      },
    },
  },
}
