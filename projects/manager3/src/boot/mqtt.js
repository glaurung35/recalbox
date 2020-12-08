import mqtt from 'mqtt'

let options = {
  clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8)
}

let client  = mqtt.connect(process.env.MQTT_URL, options)
console.log(options, client)
// client.subscribe('Recalbox/WebAPI/SystemInfo')
// client.on('message', (topic, message) => {
//   console.log(message)
// })
