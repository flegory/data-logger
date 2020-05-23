
#include <DHT.h>
#include <SD.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define DHTPIN 3     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define SWITCH_PIN 2 // switch is connected to pin 2

#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8

#define BLACK   0x0000
#define ORANGE  0xfC00
#define RED     0xf800
#define BLUE    0x001f
#define GREEN   0x07e0
#define YELLOW  0xffe0
#define PURPLE  0xF81F
#define PINK    0xF810
#define WHITE   0xffff

int Record = 1;
const int chipSelect = 4;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

unsigned long previousMillis = 0;
const unsigned long interval = 10000;
bool run;
float lastTempFahrenheit = 0.0;
float lastHumidity = 0.0;
float lastHeatIndex = 0.0;
int lastRecord = 0;

// long minutes(long x){
//   return 60L*1000L*x;
// }

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("Working...");
  dht.begin();
  // pinMode(SWITCH_PIN, INPUT); // Set the switch pin as input
  // run = false; //starts stopped

  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_BLACK);

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("Card Initialized.");
}

void loop() {

            unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        
  // code you always run here; you can leave this section blank if you want the
  // entire program to stop and start, or add code here if you want it to
  // always run

  // check button press here and if it is pressed then toggle run variable
  // between 0 and 255; REQUIRED!

  // funcitons based off of button pulling input pin LOW
  // if(digitalRead(SWITCH_PIN) == LOW)
  // {
  //    if(run)
  //    {
  //       run = false;
  //    }
  //    else
  //    {
  //       run = true;
  //    }
  // }

  // if(run)
  // {
  // code you only run if button was pressed, stops running when button
  // pressed again, so forth...

  // int switch_value = digitalRead(SWITCH_PIN);   // read input value and store
  //                                               // it in switchpin

  tft.invertDisplay(false);

  // Label Temperature
  tft.setTextColor(ORANGE);
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  tft.println("Temperature:");

  float tempFahrenheit = dht.readTemperature(true);

  // clear the last temp
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 25);
  tft.println(lastTempFahrenheit);

  // print the current temp
  tft.setTextColor(PURPLE);
  tft.setTextSize(2);
  tft.setCursor(10, 25);
  tft.println(tempFahrenheit);

  //write temp reading to to SD card
  File sdcard_file = SD.open("data.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (sdcard_file) {
    sdcard_file.print("Temperature: ");
    sdcard_file.print(tempFahrenheit);
    sdcard_file.close();

    // update last temp
    lastTempFahrenheit = tempFahrenheit;

    // Label Humidity
    tft.setTextColor(BLUE);
    tft.setTextSize(1);
    tft.setCursor(10, 45);
    tft.println("Humidity:");

    float Humidity = dht.readHumidity(true);

    // clear the last humidity
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 60);
    tft.println(lastHumidity);

    // print the current humidity
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.setCursor(10, 60);
    tft.println(Humidity);

    File sdcard_file = SD.open("data.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (sdcard_file) {
      sdcard_file.print(" Humidity: ");
      sdcard_file.print(Humidity);
      sdcard_file.close();

      // update last humidity
      lastHumidity = Humidity;

      // Label Heat Index
      tft.setTextColor(WHITE);
      tft.setTextSize(1);
      tft.setCursor(10, 80);
      tft.println("Heat Index:");

      float HeatIndex = dht.computeHeatIndex(tempFahrenheit, Humidity);

      // clear the last heat index
      tft.setTextColor(BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 95);
      tft.println(lastHeatIndex);

      // print the current heat index
      tft.setTextColor(YELLOW);
      tft.setTextSize(2);
      tft.setCursor(10, 95);
      tft.println(HeatIndex);

Record++;

      File sdcard_file = SD.open("data.txt", FILE_WRITE);
      // if the file is available, write to it:
      if (sdcard_file) {
        sdcard_file.print(" Heat Index: ");
        sdcard_file.println(HeatIndex);
        sdcard_file.close();
      }
      // update last heat index
      lastHeatIndex = HeatIndex;

      // Label "Record
      tft.setTextColor(WHITE);
      tft.setTextSize(1);
      tft.setCursor(10, 115);
      tft.println("Record #");
         
         // clear the "record"
         
      tft.setTextColor(BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 125);
      tft.println(lastRecord);
      // print the current "record"
      tft.setTextColor(PINK);
      tft.setTextSize(2);
      tft.setCursor(10, 125);
      tft.println(Record);
       lastRecord = Record;
       
      }
    }
  }
}

