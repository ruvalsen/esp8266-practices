#include "Ubidots.h"

const char* WIFI_SSID = "";           // Put here your Wi-Fi SSID
const char* WIFI_PASS = "";           // Put here your Wi-Fi password
const char* UBIDOTS_TOKEN = "";  // Put here your Ubidots TOKEN

float voltajeEntrada = 3.3;
float conversion = voltajeEntrada / 1023;

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

void setup() {
  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
}

void loop() {
  //Generar temperatura
  float temperatura = (analogRead(A0) * conversion) * 100;
  Serial.println("");
  Serial.println(temperatura);
  Serial.print("ADC value: "); Serial.println(analogRead(A0));
  
  ubidots.add("temperatura", temperatura);  // Change for your variable name

  bool bufferSent = false;
  bufferSent = ubidots.send();  // Will send data to a device label that matches the device Id

  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }
}
