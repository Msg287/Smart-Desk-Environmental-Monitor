#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// Wi-Fi information

const char* WIFI_NAME = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

WebServer server(80);

// OLED settings

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);


// BME280 settings

Adafruit_BME280 bme;

#define BME_ADDRESS 0x76

float temperatureF = 0.0;
float humidity = 0.0;

String roomStatus = "Starting";


// Determine room status

void updateRoomStatus()
{
  if (temperatureF > 78.0)
  {
    roomStatus = "Too hot";
  }
  else if (temperatureF < 65.0)
  {
    roomStatus = "Too cold";
  }
  else if (humidity > 65.0)
  {
    roomStatus = "Too humid";
  }
  else if (humidity < 25.0)
  {
    roomStatus = "Too dry";
  }
  else
  {
    roomStatus = "Comfortable";
  }
}


// Read sensor

void readSensor()
{
  float temperatureC = bme.readTemperature();

  temperatureF =
    temperatureC * 9.0 / 5.0 + 32.0;

  humidity = bme.readHumidity();

  updateRoomStatus();

  Serial.print("Temperature: ");
  Serial.print(temperatureF);
  Serial.println(" F");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Status: ");
  Serial.println(roomStatus);

  Serial.println();
}


// Update OLED

void updateDisplay()
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("SMART DESK MONITOR");

  display.drawLine(
    0,
    11,
    127,
    11,
    SSD1306_WHITE
  );

  display.setCursor(0, 18);
  display.println("Temperature");

  display.setTextSize(2);
  display.setCursor(0, 29);
  display.print(temperatureF, 1);
  display.println(" F");

  display.setTextSize(1);
  display.setCursor(76, 18);
  display.println("Humidity");

  display.setTextSize(2);
  display.setCursor(76, 29);
  display.print(humidity, 0);
  display.println("%");

  display.setTextSize(1);
  display.setCursor(0, 55);
  display.print("Status: ");
  display.println(roomStatus);

  display.display();
}


// Create webpage

String createWebpage()
{
  String page = "";

  page += "<!DOCTYPE html>";
  page += "<html>";
  page += "<head>";

  page += "<meta name='viewport' ";
  page += "content='width=device-width, initial-scale=1'>";

  page += "<meta http-equiv='refresh' content='3'>";

  page += "<title>Smart Desk Monitor</title>";

  page += "<style>";

  page += "body {";
  page += "font-family: Arial, sans-serif;";
  page += "background: #eeeeee;";
  page += "text-align: center;";
  page += "padding: 30px;";
  page += "}";

  page += ".card {";
  page += "background: white;";
  page += "max-width: 500px;";
  page += "margin: auto;";
  page += "padding: 30px;";
  page += "border-radius: 18px;";
  page += "box-shadow: 0 4px 12px rgba(0,0,0,0.15);";
  page += "}";

  page += ".reading {";
  page += "font-size: 28px;";
  page += "margin: 25px;";
  page += "}";

  page += ".status {";
  page += "font-size: 24px;";
  page += "font-weight: bold;";
  page += "}";

  page += "</style>";
  page += "</head>";

  page += "<body>";
  page += "<div class='card'>";

  page += "<h1>Smart Desk Monitor</h1>";

  page += "<div class='reading'>";
  page += "Temperature: ";
  page += String(temperatureF, 1);
  page += " &deg;F";
  page += "</div>";

  page += "<div class='reading'>";
  page += "Humidity: ";
  page += String(humidity, 1);
  page += "%";
  page += "</div>";

  page += "<div class='status'>";
  page += roomStatus;
  page += "</div>";

  page += "<p>Updates every three seconds</p>";

  page += "</div>";
  page += "</body>";
  page += "</html>";

  return page;
}


// Handle browser request

void handleMainPage()
{
  server.send(
    200,
    "text/html",
    createWebpage()
  );
}


// Setup

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS))
  {
    Serial.println("OLED failed.");

    while (true)
    {
      delay(1000);
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Starting...");
  display.display();

  if (!bme.begin(BME_ADDRESS))
  {
    Serial.println("BME280 failed.");

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("BME280 ERROR");
    display.display();

    while (true)
    {
      delay(1000);
    }
  }

  readSensor();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connecting to Wi-Fi...");
  display.display();

  WiFi.begin(
    WIFI_NAME,
    WIFI_PASSWORD
  );

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");

  Serial.print("Open this address: http://");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Wi-Fi connected!");
  display.println();
  display.println("Open address:");
  display.println(WiFi.localIP());
  display.display();

  server.on("/", handleMainPage);
  server.begin();

  delay(5000);
}


// Main loop

void loop()
{
  server.handleClient();

  readSensor();
  updateDisplay();

  delay(2000);
}