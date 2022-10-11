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
  // Serial.println("Connected to wifi network");
}

void loop() {
  static unsigned long lastWrite;
  static bool dataWritten;
  static String s;
  String message = "";
  static bool wait = false;

  //Read in data from Arduino
  while (Serial.available() > 0) {
        char c = Serial.read();
        s += c;
        //Serial.println(s);
        if (c == '\n') {
          //Serial.println("Done Reading");
          message = s;
          s = "";
        }
  }
  lastWrite = millis();
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  Serial.println(message);
  if (message.length() > 0 && !client.connect(VS_ADDRESS, VS_PORT)) {
    Serial.write("connection failed");
    return;
  } 

  // // This will send a string to the server
  if (client.connected() && message.length() > 0) { 
    //Serial.println("send data");
    client.println(message); 
  }
  // wait for data to be available
  unsigned long timeout = millis();
  if(message.length()> 0){
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        client.stop();
        delay(3000);
        return;
      }
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  //Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.write(ch);
    
  }
  lastWrite = millis();
  // Close the connection
  // client.stop();


  // Clear the write buffer after timeout
  ESP.restart();
}



