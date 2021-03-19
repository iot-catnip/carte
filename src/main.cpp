/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries for sensors
#include <SPI.h>
#include <DHT.h>
#include <ACS712.h>
#include "max6675.h"

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ------------------- PINS ----------------------
// MAX6575 module (T° sensor)
#define thermo_CLK 18
#define thermo_DO 19
#define thermo_CS 23

// DHT11 module (T° + humidity sensor)
#define DHTPIN 17
#define DHTTYPE DHT11

// ACS712 module (current sensor)
#define ACSPIN 35

// OLED pins
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

MAX6675 thermocouple(thermo_CLK, thermo_CS, thermo_DO);

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
  
  //SPI LoRa pins
  //SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  //LoRa.setPins(SS, RST, DIO0);
  
  display.setCursor(0,0);
  display.print("Initialisation");
  display.display();

  dht.begin();
}


void loop() {
  // Délai entre chaque mesure.
  delay(4000);

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
  /* display.print("Humidity: ");
  display.println(measurement.humidity);*/

   /// Tmeperature thermocouple
  display.print("Temperature: ");
  display.println(thermocouple.readCelsius());
  
  display.display();
  
  
}
