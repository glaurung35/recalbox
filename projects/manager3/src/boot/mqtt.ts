/**
 * @author Nicolas TESSIER aka Asthonishia
 */
import mqtt from 'mqtt';
import { useMonitoringStore } from 'stores/monitoring';

const options:{clientId:string} = {
  clientId: `mqttjs_${Math.random().toString(16).substring(2, 8)}`,
};

let mqttUrl = process.env.MQTT_URL;

if (process.env.MQTT_URL === '') {
  mqttUrl = `mqtt://${window.location.hostname}:18833`;
}

const client:mqtt.MqttClient = mqtt.connect(String(mqttUrl), options);
const monitoringStore = useMonitoringStore();

// SystemInfo subcription and VueJS store injection
client.subscribe(String(process.env.MQTT_MONITORING_CHANNEL));
client.on('message', (topic:string, message:Buffer):void => {
  const newMessage = JSON.parse(new TextDecoder('utf-8').decode(message));
  const temperatures:{x:string, y:string}[] = [
    // eslint-disable-next-line @typescript-eslint/ban-ts-comment
    // @ts-ignore
    ...monitoringStore.metrics.temperatures,
    [
      (new Date(newMessage.timestamp)).getTime(),
      parseFloat(newMessage.temperature.temperatures.shift().toFixed(0)),
    ],
  ];

  const memory:{x:string, y:string}[] = [
    // eslint-disable-next-line @typescript-eslint/ban-ts-comment
    // @ts-ignore
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

  const cores:{x:string, y:number}[] = Object.keys(newMessage.cpus).map(
    (core:string):{x:string, y:number} => ({
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
});
