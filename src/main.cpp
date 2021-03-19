/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries for sensors
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
#define thermo_CLK 18
#define thermo_DO 19
#define thermo_CS 23

// DHT11 module (T° + humidity sensor)
#define DHTPIN 17

// ACS712 module (current sensor)
#define ACSPIN 35

// OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//-------------------- END PINS -----------------------

// Initialisation écran OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

// Initialisation capteur DHT11
float temperature = 55;
float bTemperature = 30;
DHT dht(DHTPIN, DHT11);

// Initialisation capteur thermocouple
MAX6675 thermocouple(thermo_CLK, thermo_CS, thermo_DO);

// Initialisation capteur courant
ACS712 acsCurrent(ACS712_20A, ACSPIN);

MAX6675 thermocouple(thermo_CLK, thermo_CS, thermo_DO);

//------------------- WIFI ----------------------
#define SSID "Freebox-017025"
#define PASSWORD "obruentes-pudendum-addicerent#2-elegea94"
//------------------END WIFI----------------------

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

  // initialisation DHT
  dht.begin();

  // initialisation ACS
  int zero = acsCurrent.calibrate();
}


void loop() {
  // Délai entre chaque mesure.
  delay(4000);

  display.clearDisplay();

  // Température DHT11
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

  // Humidité DHT11
  display.print("Humidity: ");
  display.println(dht.readHumidity());

  // Temperature thermocouple
  display.print("Temperature: ");
  display.println(thermocouple.readCelsius());

  // Mesure courant
  float courant = acsCurrent.getCurrentAC();
  display.print("Courant: ");
  display.println(courant);
  display.print("Puissance: ");
  display.print(courant*230);
  
  display.println("Connected to WiFi");
  display.println(WiFi.localIP());
  display.display();

  socket.restoreConnexion();
  socket.checkForRequest();
  socket.disconect();
}
