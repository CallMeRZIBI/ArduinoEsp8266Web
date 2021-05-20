// This project was highly copied from this - https://www.youtube.com/watch?v=6-RXqFS_UtU

#include <ArduinoJson.h>

String message = "";
bool messageReady = false;

int Led = 4;
int Button = 9;

bool ledStatus = false;

void setup() {
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  pinMode(Led, OUTPUT);
  pinMode(Button, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  led();
  while(Serial.available()){
      message = Serial.readString();
      messageReady = true;
    }

  if(messageReady){
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc,message);
      if(error){
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.c_str());
          messageReady = false;
          return;
        }
      // Request from ESP8266
      if(doc["type"] == "request"){
          doc["type"] = "response";
          doc["led"] = ledStatus;
          serializeJson(doc, Serial);
        }
      messageReady = false;
    }
}

void led(){
    if(digitalRead(Button)){
    if(ledStatus){
        digitalWrite(Led, LOW);
        ledStatus = !ledStatus;
       }else{
        digitalWrite(Led, HIGH);
        ledStatus = !ledStatus;
       }
    }
  }
