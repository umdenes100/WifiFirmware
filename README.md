# WifiFirmware

Firmware to be uploaded to ESP8266 modules to interact with Vision System v5.1.x over WiFi.

## Programming the ESP8266

### First-Time Setup (Software)

Open the Arduino IDE and go to Preferences (**Ctrl + ,** on Windows or **⌘,** on macOS). In the textbox for "Additional Boards Manager URLs", insert `http://arduino.esp8266.com/stable/package_esp8266com_index.json`, then press OK. Update the board definitions by opening the Boards Manager (**Tools > Board > Boards Manager...**), finding "esp8266" on the list, selecting the latest version, and pressing Install.

### Hardware Setup

The ESP8266 can be connected to a computer through a dedicated USB converter, or using a generic USB-to-TTL converter (or an Arduino can be used as a makeshift USB-to-TTL converter using pins 0 and 1).

With a dedicated USB converter, the ESP8266 can be plugged in directly to the programming computer. Be sure that the converter is in programming mode (if it has more than one mode).

For a generic USB-to-TTL converter, the ESP8266 can be wired for progamming with a level shifter (or using voltage dividers if a level shifter is not available). **The ESP8266 has an operating voltage of 3.3 V and is NOT 5 V tolerant.** For programing, connect VCC and enable to power, RX and TX to their appropriate inputs, and GND and GPIO0 to ground.

Note: The [ESP82366](https://user-images.githubusercontent.com/32310882/64036322-e7ceea00-cb20-11e9-8db5-5e38aa3ca02b.png) is the small black chip in the wifi module. The larger [blue chip](https://user-images.githubusercontent.com/32310882/64036213-a6d6d580-cb20-11e9-873c-5798a7c38486.png) converts the voltage from 5 V down to 3.3 V, enabling the blue chip's pins to be plugged into a 5 V source.

### Uploading Code

Open the sketch from this repository in Arduino IDE. Set the board type by selecting **Tools > Board > Generic ESP8266 Module**. Set the flash mode by selecting **Tools > Flash Mode > DIO**. Set the memory size by selecting **Tools > Flash Size > 1M (no SPIFFs)**. Set the appropriate port and click upload.

_Note: The ESP8266 may need to be reset between code uploads. Simply disconnect and reconnect the VCC connection._
