/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import { apiUrl } from 'boot/axios';
import mqtt from 'mqtt';
import { systemsMetaData } from 'src/utils/systemsMetaData';
import { useEmulationstationStore } from 'stores/configuration/emulationstation';
import { useMonitoringStore } from 'stores/monitoring';
import { useSystemsStore } from 'stores/systems';
import { CurrentSystemMetaData } from 'stores/types/emulationstation';
import { EsResponse } from 'stores/types/mqtt';

const options: { clientId: string } = {
  clientId: `mqttjs_${Math.random().toString(16).substring(2, 8)}`,
};

let mqttUrl = process.env.MQTT_URL;

if (process.env.MQTT_URL === '') {
  mqttUrl = `mqtt://${window.location.hostname}:18833`;
}

const client = mqtt.connect(String(mqttUrl), options);
const monitoringStore = useMonitoringStore();
const emulationStationStore = useEmulationstationStore();

const api: string|undefined = apiUrl;

export const getCurrentSystemMetaData = (systemId: string): CurrentSystemMetaData => systemsMetaData[
  systemId as keyof typeof systemsMetaData
];

// Subcriptions and VueJS store injection
client.subscribe(String(process.env.MQTT_MONITORING_CHANNEL));
client.subscribe(String(process.env.MQTT_ES_EVENTS_CHANNEL));

client.on('message', (topic, message): void => {
  if (topic === process.env.MQTT_MONITORING_CHANNEL) {
    const newMessage = JSON.parse(new TextDecoder('utf-8').decode(message));
    const temperatures = [
      ...monitoringStore.metrics.temperatures,
      [
        (new Date(newMessage.timestamp)).getTime(),
        parseFloat(newMessage.temperature.temperatures.shift().toFixed(0)),
      ],
    ];

    const memory = [
      ...monitoringStore.metrics.memory,
      [
        (new Date(newMessage.timestamp)).getTime(),
        parseFloat(
          ((
            (newMessage.memory.total - newMessage.memory.available.shift()) / newMessage.memory.total
          ) * 100).toFixed(0),
        ),
      ],
    ];

    const cores: { x: string, y: number }[] = Object.keys(newMessage.cpus).map(
      (core): { x: string, y: number } => ({
        x: `Core ${core}`,
        y: parseFloat(newMessage.cpus[core].consumption.shift().toFixed(0)),
      }),
    );

    monitoringStore.$patch({
      // eslint-disable-next-line @typescript-eslint/ban-ts-comment
      // @ts-ignore
      metrics: {
        cores,
        temperatures: temperatures.slice(-30),
        memory: memory.slice(-30),
      },
    });
  }

  if (topic === process.env.MQTT_ES_EVENTS_CHANNEL) {
    const newMessage: EsResponse = JSON.parse(new TextDecoder('utf-8').decode(message));
    const systemsStore = useSystemsStore();
    const { systems } = systemsStore;
    let { currentSystem } = emulationStationStore.currentState;
    let currentRom = null;

    if (newMessage.System) {
      const storeSystem = systems.systems.filter((system) => system.name === newMessage.System.SystemId)[0];

      emulationStationStore.resetCurrentSystem();
      currentSystem = {
        logoPath: `${api}/systems/${storeSystem.name}/resource/eu/svg/logo`,
        name: newMessage.System.System,
        systemId: newMessage.System.SystemId,
        metaData: systemsMetaData[newMessage.System.SystemId],
      };
    }

    if (newMessage.Action === 'rungame' && newMessage.Game) {
      currentRom = {
        name: newMessage.Game.Game,
        imagePath: newMessage.Game.ImagePath,
        thumbnailPath: newMessage.Game.ThumbnailPath,
        videoPath: newMessage.Game.VideoPath,
        developer: newMessage.Game.Developer,
        publisher: newMessage.Game.Publisher,
        players: newMessage.Game.Players,
        region: newMessage.Game.Region,
        genre: newMessage.Game.Genre.replace(',', ', '),
      };
    }

    const currentState = {
      currentSystem,
      currentRom,
    };

    emulationStationStore.$patch({
      currentState,
    });
  }
});
