import mqtt from 'mqtt'
import Store from '../store'

let options = {
  clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8)
}

let client  = mqtt.connect(process.env.MQTT_URL, options)

// SystemInfo subcription and VueJS store injection
client.subscribe(process.env.MQTT_MONITORING_CHANNEL)
client.on('message', (topic, message) => {
  Store.dispatch('monitoring/updateMetrics', JSON.parse(new TextDecoder("utf-8").decode(message)))
})
