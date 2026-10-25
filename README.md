# Smart-Sesk-Environmental-Monitor
ESP32-based environmental monitor with an OLED display and Wi-Fi dashboard

Smart Desk Environmental Monitor

A personal embedded-systems project built using an ESP32, BME280 environmental sensor, and SSD1306 OLED display.

This project collects environmental measurements, displays them locally on an OLED screen, determines the room's comfort status, and hosts a Wi-Fi webpage that can be seen from a phone or computer on the same network.


Features
- Real-time temperature and humidity monitoring
- SSD1306 OLED display
- Room comfort classification
- ESP32 Wi-Fi connectivity
- Embedded HTTP web server
- Live browser-based dashboard
- Serial monitoring for debugging
- Independent USB power


Hardware
- ESP32 development board
- BME280 environmental sensor
- SSD1306 128x64 OLED display
- Breadboard
- Jumper wires
- USB-C cable
  

Technologies
- C++
- ESP32
- Arduino IDE
- I2C
- Wi-Fi
- HTTP
- HTML
- Serial communication


System Architecture

The BME280 sensor communicates with the ESP32 using the I2C protocol.

The ESP32 processes the sensor measurements and sends the data to two interfaces:

A local SSD1306 OLED display
A Wi-Fi-hosted webpage
        BME280
          |
          | I2C
          v
        ESP32
       /     \
      /       \
   OLED       Wi-Fi
  Display   Web Dashboard


Wiring

Component	  Pin 	ESP32
BME280	    VCC	  3V3
BME280	    GND	  GND
BME280	    SDA	  GPIO 21
BME280	    SCL	  GPIO 22
OLED	      VCC	  3V3
OLED	      GND	  GND
OLED	      SDA	  GPIO 21
OLED	      SCL	  GPIO 22

Both devices share the same SDA and SCL lines because they communicate using I2C.


Room Status Logic

The ESP32 classifies room conditions using temperature and humidity thresholds.

- Above 78°F: Too Hot
- Below 65°F: Too Cold
- Above 65% humidity: Too Humid
- Below 25% humidity: Too Dry
- Otherwise: Comfortable


Wi-Fi Dashboard

The ESP32 runs a local HTTP server on port 80. After connecting to Wi-Fi, the ESP32 prints its local IP address through the Serial Monitor.
A phone or computer connected to the same Wi-Fi network can open that IP address in a browser to view:

- Temperature
- Humidity
- Room comfort status

The webpage automatically refreshes every three seconds.


Setup

1. Install Arduino IDE.
2. Install the ESP32 board package.
3. Install:
   - Adafruit BME280 Library
   - Adafruit Unified Sensor
   - Adafruit GFX Library
   - Adafruit SSD1306
4. Connect the BME280 and OLED to the ESP32.
5. Open SmartDeskMonitor.ino.
6. Replace:
   
const char* WIFI_NAME = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
with your local Wi-Fi credentials.

7. Select ESP32 Dev Module.
8. Upload the program.
9. Open Serial Monitor at 115200 baud.
10. Open the ESP32's displayed IP address from a device on the same network.


Debugging

One of the biggest parts of this project was debugging sensor and I2C communication.

I used:

Serial monitoring
I2C address scanning
Sensor address testing
Continuity testing
Breadboard wiring checks
Hardware isolation
Component replacement

This helped distinguish between firmware problems, connection problems, and faulty sensor hardware.


Skills Developed

- Embedded C++
- ESP32 development
- I2C communication
- Sensor integration
- OLED interfacing
- Wi-Fi networking
- Embedded web servers
- HTML
- Soldering
- Breadboard prototyping
- Serial debugging
- Hardware troubleshooting


Future Improvements

- Log measurements over time
- Graph historical temperature and humidity data
- Add additional environmental sensors
- Build a custom PCB
- Design a 3D-printed enclosure
- Add remote access beyond the local Wi-Fi network

