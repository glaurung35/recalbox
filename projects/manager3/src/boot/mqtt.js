import mqtt from 'mqtt';
import { useMonitoringStore } from 'stores/monitoring';

const options = {
  clientId: `mqttjs_${Math.random().toString(16).substr(2, 8)}`,
};
const client = mqtt.connect(process.env.MQTT_URL, options);
const monitoringStore = useMonitoringStore();

// SystemInfo subcription and VueJS store injection
client.subscribe(process.env.MQTT_MONITORING_CHANNEL);
client.on('message', (topic, message) => {
  const newMessage = JSON.parse(new TextDecoder('utf-8').decode(message));
  const temperatures = [
    ...monitoringStore.metrics.temperatures,
    [
      (new Date(newMessage.timestamp)).getTime(),
      newMessage.temperature.temperatures.shift().toFixed(0),
    ],
  ];
  const cores = Object.keys(newMessage.cpus).map(
    (core) => newMessage.cpus[core].consumption.shift().toFixed(0),
  );
  monitoringStore.$patch({
    metrics: {
      cores,
      temperatures: temperatures.slice(-30),
    },
  });
});
