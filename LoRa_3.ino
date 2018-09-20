#include <SoftwareSerial.h>
#include <TheThingsNetwork.h>


// Set your DevAddr, NwkSKey, AppSKey and the frequency plan

const char *devAddr = "07ed57dd";
const char *nwkSKey = "1ba5086ec4d7bb83924f1dc18ffae70a";
const char *appSKey = "d595ef0851bf4f37760e46c4fd7ea477";

#define debugSerial Serial
const int buttonPin = 6;
int ledState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin
String string;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

SoftwareSerial loraSerial(2,3);
// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_KR920_923

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(115200);
  pinMode(buttonPin, INPUT);
  pinMode(7, OUTPUT);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- PERSONALIZE");
  ttn.personalize(devAddr, nwkSKey, appSKey);

  debugSerial.println("-- STATUS");
  ttn.showStatus();
}

void loop()
{
  debugSerial.println("-- LOOP");
  int reading = digitalRead(buttonPin);


  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;

      digitalWrite(7, ledState);
      Serial.println(ledState);

      if (ledState==1){
        string = "1 ";
      }
      else{
        string = "0 ";
      }
  
    byte payload[2];
    string.getBytes(payload, string.length());
    //payload[0] = (digitalRead(ledState == HIGH) ? 1 : 0);
    // Send it off
    ttn.sendBytes(payload, sizeof(payload));
      }
    }
  }

  

  // set the LED:


  uint32_t t1 = millis();
  //Prepare payload of 1 byte to indicate LED status

 

  uint32_t t2 = millis();
  Serial.println(t2-t1);  

    // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  

}
