/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import mqtt from 'mqtt';
import { systemsMetaData } from 'src/utils/systemsMetaData';
import { useEmulationstationStore } from 'stores/configuration/emulationstation';
import { useMonitoringStore } from 'stores/monitoring';
import { CurrentSystemMetaData } from 'stores/types/emulationstation';
import { Actions, EventResponse } from 'stores/types/mqtt';

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

export const getCurrentSystemMetaData = (systemId: string): CurrentSystemMetaData => systemsMetaData[
  systemId as keyof typeof systemsMetaData
];

// Subcriptions and VueJS store injection
client.on('connect', () => {
  emulationStationStore.fetchStatus();
  client.subscribe(String(process.env.MQTT_MONITORING_CHANNEL));
  client.subscribe(String(process.env.MQTT_ES_EVENTS_CHANNEL));
});

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
    const newMessage: EventResponse = JSON.parse(new TextDecoder('utf-8').decode(message));

    if (newMessage.Action === Actions.reboot) {
      emulationStationStore.resetCurrentSystem();
    } else if (
      newMessage.Action === Actions.systemBrowsing
      || newMessage.Action === Actions.gamelistBrowsing
      || newMessage.Action === Actions.runGame
      || newMessage.Action === Actions.start
      || newMessage.Action === Actions.wakeup
      || newMessage.Action === Actions.sleep
    ) {
      emulationStationStore.updateStatus(newMessage);
    }
  }
});
