var mqtt = require('mqtt')
var client2  = mqtt.connect('mqtt://localhost')

client2.on('connect', function () {
  client2.subscribe('outTopic')
  //client2.subscribe('#')
  client2.publish('presence', 'Hello mqtt client2')
})

client2.on('message', function (topic, message){

  console.log(topic)
  var message = message.toString();
  console.log(message.trim())
  var coordinate = message.split(',');
  console.log("Lat Long :" + message)
  var downlink_coordinate = JSON.stringify({
     "_id":"123",
     "latitude": coordinate[0].trim(),
     "longitude": coordinate[1].trim(),
     "coordinate": message,
     "display":"add",
   });
   client2.publish("downlink",downlink_coordinate);
   console.log("Downlink Coordinate lat: %s long: %s", coordinate[0].trim(), coordinate[1].trim())
   console.log(' ')
});
