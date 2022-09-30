#include <Arduino.h>
#include "ESP8266WiFi.h"


// WiFi network name
#define ROOM 1215
#if ROOM == 1116  //big lab
    #define WIFI_NETWORK "VisionSystem1116-2.4"
#elif ROOM == 1215 //small Lab
    #define WIFI_NETWORK "VisionSystem1215-2.4"
#endif

unsigned char seq = 0;
String arduinoInput = "";

const char* VS_ADDRESS = "192.168.1.2";
const int VS_PORT = 7755;





void setup() {  
  // Begin serial communication with Arduino
  Serial.begin(9600);

  //size_t len = 0, size = 10;
  //char*str = (char *) malloc(size*sizeof(char));
  

  

  // Connect to Vision System network
  WiFi.begin(WIFI_NETWORK, NULL);
  // Connect to vision system
  // ws.connect(VS_ADDRESS, "/", VS_PORT);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    ESP.restart();
  }
  Serial.println("Connected to wifi network");
}

void loop() {
  static unsigned long lastWrite;
  static bool dataWritten;
  //static String s;
  static bool wait = false;


  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(VS_ADDRESS, VS_PORT)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) { client.println("hello from ESP8266"); }

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
      delay(60000);
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.write(ch);
  }

  // Close the connection
  client.stop();

  if (wait) {
    delay(300000);  // execute once every 5 minutes, don't flood remote service
  }
  wait = true;
  // Receiving input from Arduino, Sending to VS
  // if (Serial.available() > 0) {
  //       char c = Serial.read();
  //       arduinoInput += c;
  //       if (c == '\n'){
  //         char* buf = (char*) malloc(sizeof(char)*arduinoInput.length()+1);
  //         buf[arduinoInput.length()] = '\0';
  //         //s.toCharArray(buf, s.length()+1);
  //         //Serial.write(buf);
  //         // ws.send(buf);
  //         free(buf);
  //         arduinoInput = "";
  //         lastWrite = millis();
  //       }
  // }

  // Receiving from VS, Sending to Arduino
  // if (!ws.isConnected()) {
	// 	ws.connect(VS_ADDRESS, "/", VS_PORT);
	// } else {
	// 	ws.send("hello");

	// 	String msg;
	// 	if (ws.getMessage(msg)) {
  //       for (int i=0; i < msg.length(); i++) {
  //         Serial.write(msg[i]);
  //       }
	// 		  //Serial.write(msg);
	// 	}
	// }

  // Clear the write buffer after timeout
  if (millis() - lastWrite > 20) {
    ESP.restart();
  }
}



