/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries for sensors
#include <WiFi.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Project class
#include "./extention/catnip/CatNip.h"
#include "./extention/socket/ClientSocket.h"
#include "./extention/sensor/Sensors.h"
// OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//-------------------- END PINS -----------------------

// Initialisation écran OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

//------------------- WIFI ----------------------
#define SSID "Freebox-017025"
#define PASSWORD "obruentes-pudendum-addicerent#2-elegea94"
//------------------END WIFI----------------------

ClientSocket socket;
Sensors sensors;

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
  sensors.init();
}


void loop() {
  // Délai entre chaque mesure.
  delay(4000);

  display.clearDisplay();

  // Température DHT11
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.println(sensors.getTemperature());

  // Humidité DHT11
  display.print("Humidity: ");
  display.println(sensors.getHumidity());

  // Temperature thermocouple
  display.print("Temperature: ");
  display.println(sensors.getTermoTemperature());

  // Mesure courant
  display.print("Courant: ");
  display.println(sensors.getCourant());
  display.print("Puissance: ");
  display.println(sensors.getPower());
  
  // Afficher Wifi
  display.println("Connected to WiFi");
  display.println(WiFi.localIP());
  display.display();

  socket.restoreConnexion();
  socket.checkForRequest(sensors);
  socket.disconect();
}
