#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "WebSocketClient.h"


// WiFi network name
#define ROOM 1116
#if ROOM == 1116  //big lab
    #define WIFI_NETWORK "VisionSystem1116-2.4"
#elif ROOM == 1215 //small Lab
    #define WIFI_NETWORK "VisionSystem1215-2.4"
#endif

unsigned char seq = 0;
String arduinoInput = "";

const char* VS_ADDRESS = "192.168.1.2";
const int VS_PORT = 7755;
WiFiClient client;
// WebSocketClient ws(true);
void setup() {  
  // Begin serial communication with Arduino
  Serial.begin(9600);

  //size_t len = 0, size = 10;
  //char*str = (char *) malloc(size*sizeof(char));
  // Connect to Vision System network

  WiFi.begin(WIFI_NETWORK, NULL);
  // Connect to vision system
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    ESP.restart();
  }

  client.connect(VS_ADDRESS, VS_PORT);
  // Serial.println("Connected to wifi network");
}

char buff[100];
byte buff_index = 0;

void loop() {

  //Read in data from Arduino
  while (Serial.available() > 0) {
        buff[buff_index++] = Serial.read();
        if(!client.connected())
          ESP.restart();
        if (buff[buff_index-1] == '\n') {
          client.write(buff, buff_index);
          buff_index = 0; 
        }
  }

  // //Serial.println(message);
  // if (message.length() > 0 && client.connected()) {
  //   //Serial.write("connection failed");
  //   return;
  // } 

  // // // This will send a string to the server
  // if (client.connected()) { 
  //   //Serial.println("send data");
    
  // }
  // wait for data to be available
  // unsigned long timeout = millis();
 
  // while (client.available() == 0) {
  //   if (millis() - timeout > 5000) {
  //     //client.stop();
  //     delay(3000);
  //     return;
  //   }
  // }
  
  // Read all the lines of the reply from server and print them to Serial
  //Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.write(ch);
  }
  // lastWrite = millis();
  // Close the connection
  // client.stop();


  // Clear the write buffer after timeout
  //ESP.restart();
}



