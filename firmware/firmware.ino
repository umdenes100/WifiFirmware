#include <Arduino.h>
#include "WebSocketClient.h"
#include "ESP8266WiFi.h"

// WiFi network name
#define ROOM 1116
#if ROOM == 1116
    #define WIFI_NETWORK "VisionSystem1116-2.4"
#elif ROOM == 1215
    #define WIFI_NETWORK "VisionSystem1215-2.4"
#endif

unsigned char seq = 0;

const char* VS_ADDRESS = "192.168.1.2";
const int VS_PORT = 7755;
const byte FLUSH_SEQUENCE[] = {0xFF, 0xFE, 0xFD, 0xFC};

WebSocketClient ws(true);


void setup() {  
  // Begin serial communication with Arduino
  Serial.begin(9600);
  // Connect to Vision System network
  WiFi.begin(WIFI_NETWORK, NULL);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    ESP.restart();
  }
}

void loop() {
  static int opcode, pos;
  static unsigned long lastWrite;
  static bool dataWritten;

  if (!ws.isConnected()) {
		ws.connect(VS_ADDRESS, "/", VS_PORT);
	} else {
		ws.send("hello");

		String msg;
		if (ws.getMessage(msg)) {
			  Serial.println(msg);
		}
	}
	delay(500);
  // // Clear the write buffer after timeout
  // if (dataWritten && millis() - lastWrite > 20) {
  //   ESP.restart();
  // }
}



