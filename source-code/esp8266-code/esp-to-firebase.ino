#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "FirebaseESP8266.h"

const char *ssid = "SECE-TRAINING";
const char *password = "sece@123";

const char *FIREBASE_AUTH= "AIzaSyCEIIZYTV-hSUG0rc3LlOWWrCjjibOguUQ"; 
const char *FIREBASE_HOST= "https://ammonia-detection-default-rtdb.asia-southeast1.firebasedatabase.app/"; 
const char *WIFI_SSID= "SECE-TRAINING"; 
const char *WIFI_PASSWORD= "sece@123"; 

const int ammoniaPin = A0;
const int ammoniaLedPin = D4; 

WiFiClient client;
FirebaseData firebaseData;

#define LCD_ADDRESS 0x27

LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

void setup() {
  Serial.begin(115200);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  connectToWiFi();
  ThingSpeak.begin(client);

  pinMode(ammoniaLedPin, OUTPUT);

  lcd.init();
  lcd.backlight();
}

void loop() {

  int ammoniaValue = analogRead(ammoniaPin);
  float ammoniaConcentration = map(ammoniaValue, 0, 1023, 0, 100);

  Serial.println("Ammonia Concentration: " + String(ammoniaConcentration) + " ppm");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ammonia: ");
  lcd.print(ammoniaConcentration);
  lcd.print(" ppm");

  bool ledState = digitalRead(ammoniaLedPin);
  lcd.setCursor(0, 1);
  lcd.print("LED: ");
  lcd.print(ledState ? "ON " : "OFF");

  if (ammoniaConcentration > 20) {
    digitalWrite(ammoniaLedPin, HIGH);
  } else {
    digitalWrite(ammoniaLedPin, LOW);
  }
  Firebase.setString(firebaseData, "/ammoniaValues/value", ammoniaConcentration);


  delay(3000);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Failed to connect to WiFi. Please check your credentials.");
  }
}