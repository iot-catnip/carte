/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries
#include <SPI.h>
#include <DHT.h>
#include <ACS712.h>
#include "max6675.h"
#include <WiFi.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Project class
#include "./extention/catnip/CatNip.h"
#include "./extention/socket/ClientSocket.h"

// ------------------- PINS ----------------------
// MAX6575 module (T° sensor)
#define thermo_CLK 5
#define thermo_DO 19
#define thermo_CS 21

// DHT11 module (humidity sensor)
#define DHTPIN 17
#define DHTTYPE DHT11

// ACS712 module (current sensor)
#define ACSPIN 35

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//-------------------- END PINS -----------------------

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

/** Initialize DHT sensor */
float temperature = 55;
float bTemperature = 30;
DHT dht(DHTPIN, DHTTYPE);

//------------------- WIFI ----------------------
#define SSID "Freebox-017025"
#define PASSWORD "obruentes-pudendum-addicerent#2-elegea94"
//------------------END WIFI----------------------

MAX6675 thermocouple(thermo_CLK, thermo_CS, thermo_DO);

ClientSocket socket;
void setup() {
  
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  
  //initialize Serial Monitor
  Serial.begin(115200);
  
  WiFi.begin(SSID,PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Conneting to WiFi");
  }

  unsigned char mac[6];
  WiFi.macAddress(mac);
  Serial.print("mac : ");
  Serial.printf("%x-%x-%x-%x-%x-%x\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
  socket.setMacAddress(mac);
  socket.setConnexion("192.168.1.11");

  
  display.setCursor(0,0);
  display.print("Initialisation");
  display.display();

  dht.begin();  
}


void loop() {

  display.clearDisplay();

  temperature = dht.readTemperature();
  
  display.setCursor(0,0);
  display.print("Temperature: ");
  
  if(!isnan(temperature)){
    bTemperature = temperature;
    display.println(temperature);
  }
  else{
    display.println(bTemperature);
  }
  /*
  display.print("Humidity: ");
  display.println(measurement.humidity);*/

   /// Tmeperature thermocouple
  display.print("Temperature: ");
  display.println(thermocouple.readCelsius());
  display.println("Connected to WiFi");
  display.println(WiFi.localIP());
  display.display();

  socket.restoreConnexion();
  socket.checkForRequest();
  socket.disconect();
  // Délai entre chaque mesure.
  delay(5000);
}
