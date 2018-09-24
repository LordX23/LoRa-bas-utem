var lora_packet = require('lora-packet');
var mqtt = require('mqtt')
var client2  = mqtt.connect('mqtt://XXXXXXX')

client2.on('connect', function () {
  client2.subscribe('gateway/+/rx')
  //client2.subscribe('#')
  client2.publish('presence', 'Hello mqtt client2')
})

client2.on('message', function (topic, message){

  console.log(topic)
  var message = message.toString();
  console.log(message)
  var mess = message.split(',');
  //console.log(mess[0]+","+mess[7]+","+mess[8]+","+mess[15])
  var rssi= mess[7].split(':');
  var rssiVal= rssi[1];
  console.log('Decoding the message')
  mm = JSON.parse(message);
  if(mm.phyPayload){
    var packet = lora_packet.fromWire(new Buffer(XXXXXXXX, 'base64'));
    var NwkSKey = new Buffer('XXXXXXXX', 'hex');
    console.log("MIC check=" + (lora_packet.verifyMIC(packet, NwkSKey) ? "OK" : "fail"));
    var AppSKey = new Buffer('XXXXXXXXXXXX', 'hex');
    latlong = lora_packet.decrypt(packet, AppSKey, NwkSKey).toString();
    console.log("Decrypted (ASCII)='" + latlong + "'"); 
    var partsOfStr = latlong.split(',');
    console.log("sizeof partsOfStr" + partsOfStr.length)
    if(partsOfStr.length==2){

      var gps_coordinate = JSON.stringify({
        "_id":"126",
        "latitude":  partsOfStr[0].trim(),
        "longitude": partsOfStr[1].trim(),
        "coordinate": latlong,
        "display":"add",
        "rssi" : rssiVal,
      });
      client2.publish("lora-utem/coordinate",gps_coordinate);
      console.log("GPS Coordiate lat: %s long: %s", partsOfStr[0].trim(), partsOfStr[1].trim());
   }
    if(partsOfStr.length==1){

      var pm = JSON.stringify({
      "reference": "abcdxx1234",                  // reference which will be used on ack or error (this can be a r$
      "confirmed": true,                        // whether the payload must be sent as confirmed data down or not
      "fPort": 11,                              // FPort to use (must be > 0)
      "data": Buffer.from(latlong).toString('base64'),                            // base64 encoded data (plaintex$
      });

     client2.publish("XXXXXXXXXXX",pm);
     console.log("sending message to lora node by publish to XXXXXXXXXX" );

   }
  }
  console.log(' ')
});



