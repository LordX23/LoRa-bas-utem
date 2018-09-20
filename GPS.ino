#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;

SoftwareSerial ss(5,4);

float flat, flon;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ss.begin(9600);
  Wire.begin(2);
  Wire.onRequest(Request);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(ss.available()){
    gps.encode(ss.read());
    flat = gps.location.lat();
    flon = gps.location.lng();
    //Serial.print("Lat =");
    //Serial.println(flat,5);
    //Serial.print("Lon =");
    //Serial.println(flon,5);

    }
}  

void Request(){
  //if (flat < flon){
    char wdata[19];
    char data_lat[10], data_lon[10];
    dtostrf(flat,9,5,data_lat);
    dtostrf(flon,9,5,data_lon);
    
    sprintf(wdata, "%s,%s", data_lat, data_lon);

    Serial.println(wdata);
    Wire.write(wdata);
    Wire.write("\n");

}


