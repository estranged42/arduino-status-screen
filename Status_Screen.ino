/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include "Adafruit_LiquidCrystal.h"

WiFiMulti WiFiMulti;

// Connect direct with GPIO pins
//
// LCD          Feather
// --------     --------
// 4  - RS      25
// 6  - EN      26
// 14 - DB7     32
// 13 - DB6     15
// 12 - DB5     33
// 11 - DB4     27
//

#define PIN_RS 25
#define PIN_EN 26
#define PIN_DB7 32
#define PIN_DB6 15
#define PIN_DB5 33
#define PIN_DB4 27

Adafruit_LiquidCrystal lcd(PIN_RS, PIN_EN, PIN_DB4, PIN_DB5, PIN_DB6, PIN_DB7);

// Custom Character Designer
// https://www.quinapalus.com/hd44780udg.html
byte wifi1[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00100,
};

byte wifi2[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00100,
  B01010,
  B00000,
  B00100,
};

byte wifi3[8] = {
  B00000,
  B01110,
  B10001,
  B00000,
  B00100,
  B01010,
  B00000,
  B00100,
};

void setup()
{
  
  Serial.begin(115200);
  delay(10);

  lcd.begin(16, 2);
  delay(20);
  
  lcd.createChar(0, wifi1);
  lcd.createChar(1, wifi2);
  lcd.createChar(2, wifi3);

  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("Starting up");

  // We start by connecting to a WiFi network
  WiFiMulti.addAP("UAGuest", "arduinotest");

  Serial.println("");
  Serial.println("");
  Serial.println("Starting up");
  Serial.println(WiFi.macAddress());

  Serial.println();
  Serial.print("Wait for WiFi... ");

  lcd.setCursor(15, 0);
  lcd.write(byte(0));
  delay(200);
  lcd.setCursor(15, 0);
  lcd.write(byte(1));
  delay(200);
  lcd.setCursor(15, 0);
  lcd.write(byte(2));
  delay(200);
  lcd.setCursor(15, 0);
  lcd.write(byte(1));
  delay(200);
  lcd.setCursor(15, 0);
  lcd.write(byte(0));

  while(WiFiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      lcd.setCursor(15, 0);
      lcd.write(byte(0));
      delay(200);
      lcd.setCursor(15, 0);
      lcd.write(byte(1));
      delay(200);
      lcd.setCursor(15, 0);
      lcd.write(byte(2));
      delay(200);
      lcd.setCursor(15, 0);
      lcd.write(byte(1));
      delay(200);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());

  delay(500);
}


void loop()
{

    // Use WiFiClient class to create TCP connections
    HTTPClient http;

    http.begin("http://f.fischco.org/status.txt"); //HTTP

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if(httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            Serial.println(payload);

            int newLineIndex = payload.indexOf("\n");
            Serial.println(newLineIndex);
            String lineOne = payload;
            String lineTwo;
            if (newLineIndex > 0) {
              lineOne = payload.substring(0, newLineIndex);
              lineTwo = payload.substring(newLineIndex + 1);
            }
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(lineOne);
            if (newLineIndex > 0) {
              lcd.setCursor(0, 1);
              lcd.print(lineTwo);
            }
        }
    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

    delay(30000);

}

