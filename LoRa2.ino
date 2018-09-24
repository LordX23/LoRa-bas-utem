#include <TheThingsNetwork.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>


const int ledPin = 25;
const int buttonPin = 26;
#define freqPlan TTN_FP_KR920_923

// Update these with values suitable for your network.
const char *devAddr = "XXXXXX";
const char *nwkSKey = "XXXXXXXXXXXXXXXXXXX";
const char *appSKey = "XXXXXXXXXXXXXXXXXXX";


const char* ssid = "XXXXXX";
const char* password = "XXXXXX";
const char* mqtt_server = "XXXXXXXX";
int MQTTport=1883;
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
String stringOne =  String(1000, DEC);
int state = 0;
float flat, flon;
String string;
char coordinate[20];
int buttonState = 0;

TheThingsNetwork ttn(Serial2, Serial, freqPlan);
TinyGPSPlus gps;

void message(const byte* payload, size_t length, port_t port) {
  Serial.println("-- MESSAGE");
  Serial.println("Received " + String(length) + " bytes on port " + String(port) + ":");
    
  for (int i = 0; i < length; i++) {
    //Serial.print(" " + String(payload[i],HEX) + " 0x");
    //Serial.println(payload[i],HEX);
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if((char)payload[0]== '1'){
    Serial.print("publish message :");
    Serial.println(coordinate);
    client.publish("outTopic", coordinate);
    digitalWrite(ledPin,HIGH);
  }
  else if((char)payload[0]== '0'){
    Serial.print("publish message :");
    Serial.println(coordinate);
    client.publish("outTopic", coordinate);
    digitalWrite(ledPin,LOW);
  }
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

//  // Switch on the LED if an 1 was received as first character
//  if ((char)payload[0] == '1') {
//    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
//    // but actually the LED is on; this is because
//    // it is acive low on the ESP-01)
//  } else {
//    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
//  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "Hello ");
      // ... and resubscribe
      client.subscribe("Button");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);   
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
  Serial2.begin(57600);//lora
  Serial1.begin(9600, SERIAL_8N1, 22, 21);//gps
  

  setup_wifi();
  client.setServer(mqtt_server, MQTTport);
  client.setCallback(callback);  
  
  
//  while (!Serial && millis() < 10000);
  ttn.onMessage(message);
  
  Serial.println("-- PERSONALIZE");
  ttn.personalize(devAddr, nwkSKey, appSKey);
  
  Serial.println("-- STATUS");
  ttn.showStatus();
  delay(2000);

    

}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (!client.connected()) {
    reconnect();
  }

//  if(buttonState == HIGH){
//    byte payload1[2]; 
//    String button = "52";
//    button.getBytes(payload1, button.length());
//    ttn.sendBytes(payload1,sizeof(payload1));
//    Serial.println("Button Pressed");
//  }
  
   Serial.println("-- LOOP");

  while(Serial1.available()){
    gps.encode(Serial1.read());
    flat = gps.location.lat();
    flon = gps.location.lng();
  }

  char coordinate[20];
  char data_lat[10], data_lon[10];
  dtostrf(flat,9,5,data_lat);
  dtostrf(flon,9,5,data_lon);

  snprintf(coordinate,20,"%s,%s", data_lat, data_lon);
  string = String(coordinate); 
  //String string = "2.12,102.23";
  Serial.print("coordinate =");
  Serial.println(string);
  
  byte payload[string.length()];
  string.getBytes(payload, string.length());
  ttn.sendBytes(payload, sizeof(payload));
  
  ttn.poll(11,true);
  delay(2000);
  
  client.loop();

//  long now = millis();
//  if (now - lastMsg > 2000) {
//    lastMsg = now;
//    ++value;
//    snprintf (msg, 75, "hello world #%ld", value);
//    Serial.print("Publish message: ");
//    Serial.println(msg);
//    client.publish("outTopic", msg);
// 
//  }
  
}
