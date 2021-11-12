#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define D0 16
#define D6 12
float voltajeEntrada = 3.3;
float conversion = voltajeEntrada / 1023;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  Serial.begin(115200);
  lcd.init();                     
  lcd.init(); 
  pinMode(D6, OUTPUT);
  lcd.backlight();
}

void loop()
{
  float temperatura = ((analogRead(A0) * conversion) * 100);
  Serial.println(temperatura);
  delay(1000);
  
  //Temperatura
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temperatura);

  //Fotorresistor
  int sensor = digitalRead(D0);  
  
  if(sensor == 1){
    lcd.setCursor(0,1);
    lcd.print("LDR: ");
    
    lcd.setCursor(5,1);
    lcd.print(sensor);
    
    lcd.setCursor(10,1);
    lcd.print("      ");
    
    lcd.setCursor(10,1);
    lcd.print("Dia");
    digitalWrite(D6, LOW);
  }
  else{
    lcd.setCursor(0,1);
    lcd.print("LDR: ");
    
    lcd.setCursor(5,1);
    lcd.print(sensor);

    lcd.setCursor(10,1);
    lcd.print("      ");

    lcd.setCursor(10,1);
    lcd.print("Noche");
    digitalWrite(D6, HIGH);
  }
}
