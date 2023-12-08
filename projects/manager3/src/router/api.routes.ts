/**
 * @author Nicolas TESSIER aka Asthonishia
 */
export const getPathBy = (route: string, arg: string) => route.replace('*', arg);

export const GLOBAL = {
  architecture: '/architecture',
  status: '/status',
  versions: '/versions',
};

export const BIOS = {
  root: '/bios',
  all: '/bios',
  export: '/bios/download',
  upload: '/bios/upload/',
};

export const CONFIGURATION = {
  audio: '/configuration/audio',
  autorun: '/configuration/autorun',
  controllers: '/configuration/controllers',
  emustation: '/configuration/emulationstation',
  global: '/configuration/global',
  hat: '/configuration/hat',
  hyperion: '/configuration/hyperion',
  kodi: '/configuration/kodi',
  music: '/configuration/music',
  patreon: 'configuration/patron',
  scraper: '/configuration/scraper',
  screenshots: '/configuration/screenshots',
  system: '/configuration/system',
  systems: '/configuration/system/*',
  tate: '/configuration/tate',
  updates: '/configuration/updates',
  wifi: '/configuration/wifi',
  wifi2: '/configuration/wifi2',
  wifi3: '/configuration/wifi3',
};

export const SYSTEM = {
  es: {
    start: '/system/frontend/start',
    stop: '/system/frontend/stop',
    restart: '/system/frontend/restart',
  },
  reboot: '/system/reboot',
  shutdown: '/system/shutdown',
  supportArchive: '/system/supportarchive',
};

export const MEDIA = {
  root: '/media',
  all: '/media',
  get: '/media/screenshot/',
  delete: '/media/',
  takeScreenshot: '/media/takescreenshot',
};

export const MONITORING = {
  cpuInfo: '/monitoring/cpuinfo',
  storageInfo: '/monitoring/storageinfo',
};

export const ROMS = {
  root: '/roms',
  all: '/roms',
};

export const SYSTEMS = {
  root: '/systems',
  all: '/systems',
  roms: '/systems/*/roms',
};

export const TWITCH = {
  auth: 'https://id.twitch.tv/oauth2/token',
  schedule: 'https://api.twitch.tv/helix/schedule',
};
