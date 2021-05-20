// This project was highly copied from this - https://www.youtube.com/watch?v=6-RXqFS_UtU

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

ESP8266WebServer server(80);

const char* ssid = ""; //Enter Wi-Fi SSID
const char* password =  ""; //Enter Wi-Fi Password

// To send state of led from arduino to esp8266 you need to create serial connection where arduino will write and esp will read https://iot-guider.com/esp8266-nodemcu/serial-communication-between-esp8266-and-arduino/
 
void setup() {   
  Serial.begin(9600); //Begin Serial at 9600 Baud
  WiFi.begin(ssid, password);  //Connect to the WiFi network
  
  while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
      delay(500);
      Serial.println("Waiting to connect...");
      
      // debug
      //digitalWrite(Led, LOW);
  }
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Print the local IP
  
  server.on("/", handle_index); //Handle Index page
  
  server.begin(); //Start the server
  Serial.println("Server listening");
}

void loop() {
  server.handleClient(); //Handling of incoming client requests
}

void handle_index() {
  // Send request
  DynamicJsonDocument doc(1024);
  bool led = false;
  doc["type"] = "request";
  serializeJson(doc, Serial);

  // Response
  bool messageReady = false;
  String message = "";
  while(messageReady == false){
      if(Serial.available()){
          message = Serial.readString();
          messageReadyhttps://www.youtube.com/watch?v=6-RXqFS_UtU = true;
        }
    }
  DeserializationError error = deserializeJson(doc, message);
  if(error){
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
    }
    led = doc["led"];
    String output = led == true ? "Led is on!" : "Led is off!";

    server.send(200, "text/plain", output);
}

// To install it flash it on ESP8266 you must have GPIO0 pluged to ground and need to have reset of arduino connected to ground (sometimes it wont work), and as board while flashing set ESP8266.
// When it's flashed you need to remove the GPIO0 connection from ground on ESP8266 and connect reset of ESP8266 to ground for 1 second and disconnect, and it will start
// You then need to unconnect reset pin of arduino and switch RX and TX so RX of ESP8266 will be connected to TX of Arduino and TX of ESP8266 to RX of Arduino.
