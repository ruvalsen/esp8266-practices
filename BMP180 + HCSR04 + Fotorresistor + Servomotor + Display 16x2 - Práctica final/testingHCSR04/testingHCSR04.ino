/*
Este programa usa ciertos valores de los sensores
BMP180, HCSR04 y Fotorresistor, para ser impresos 
en un display 16x2. Luego se simula un relay para
controlar el funcionamiento de un servomotor según
la temperatura que exista.
*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <HCSR04.h>

//Display
float voltajeEntrada = 3.3;
float conversion = voltajeEntrada / 1023;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Distance
const int trigPin = 02;  //D4
const int echoPin = 00;  //D3
long duration;
float distance;

//BMP180
#include <SFE_BMP180.h>
#include <Wire.h>
SFE_BMP180 bmp180;

//Servomotor
#include <Servo.h>

//Servomotor
Servo servoMotor;

//LEDs
#define D8 15
#define D7 13


void setup() {
  //Display
  Serial.begin(115200);
  lcd.init();                     
  lcd.init(); 
  lcd.backlight();

  //Distance
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  //Iniciar el BMP180
  if (bmp180.begin()) Serial.println("BMP180 init success");
  else Serial.println("BMP180 init fail\n\n");

  //Servomotor
  servoMotor.attach(12);
  
  //LEDs
  pinMode(D8, OUTPUT);
  pinMode(D7, OUTPUT);
}

void loop() {
  //hcsr04
  hcsr04();

  //BMP180
  bmp180Sensor();

  //Fotorresistor
  fotorresistor();
    
  delay(2000);
  lcd.clear();
}

void printDisplay(int x, int y, String value, bool isNumber){
    lcd.setCursor(x,y);
    lcd.print(value);       
}

void hcsr04(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  Serial.println(duration);
  
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  int d = distance + 0.5;
  printDisplay(0,1,(String)d,false);
  printDisplay(2,1, "cm", true);
}

void bmp180Sensor(){
  //BMP180
  char status;
  double temp, pres, pSeaLevel, alt;

  Serial.println("BMP180");

  status = bmp180.startTemperature();
  if (status != 0){
    delay(status);
    status = bmp180.getTemperature(temp);
    
    if (status != 0){
      Serial.print("Temperature: ");
      Serial.print(temp,2);
      Serial.println(F("°C"));
      
      status = bmp180.startPressure(3);
      if (status != 0){
        delay(status);    
        status = bmp180.getPressure(pres, temp);
        
        if (status != 0){
          Serial.print("Abs. pressure: ");
          Serial.print(pres, 2);
          Serial.println(" mb");     

          pSeaLevel = bmp180.sealevel(pres, 470);
          Serial.print("Rel. pressure: ");
          Serial.print(pSeaLevel,2);
          Serial.println(" mb");              

          alt = bmp180.altitude(pres, pSeaLevel);
          Serial.print("Altitude: ");
          Serial.print(alt,2);
          Serial.println(" m");  

          //Temp
          int t = temp + 0.5;
          printDisplay(0,0,(String)t,false);
          printDisplay(2,0,"C",false);

          //Pres
          int p = pres + 0.5;
          printDisplay(4,0,(String)p,false);
          printDisplay(7,0,"mb",false);          
          
          //Alt
          int a = alt + 0.5;
          printDisplay(10,0,(String)a,false);
          printDisplay(13,0,"m",false);
          
          printDisplay(10, 1, "R2:", false);
          //Servomotor
          if(temp >= 29){
            servoMotor.write(360);
            printDisplay(13, 1, "1", false);
          }
          else{
           servoMotor.write(0); 
           printDisplay(13, 1, "0", false);
          }
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}

void fotorresistor(){
  float sensor = analogRead(A0);
  Serial.print("Fotorresistor: ");
  Serial.println(sensor);

  //Convertir valores del sensor a voltaje
  float voltaje = sensor * (5.0 / 1023.0);
  Serial.print("F.V.: ");
  Serial.println(voltaje);

  printDisplay(5, 1, "R1:", false);
  
  if(voltaje <= 3.0){
    digitalWrite(D8, HIGH);
    Serial.println("R1: ON");
    printDisplay(8,1,"1",false);    
  }
  else{
    digitalWrite(D8, LOW);
    Serial.println("R1: OFF");
    printDisplay(8,1,"0",false);
  }
  
}
