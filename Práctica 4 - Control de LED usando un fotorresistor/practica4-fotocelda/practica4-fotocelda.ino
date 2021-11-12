#define D4 LED_BUILTIN
#define D0 16

void setup() {
  Serial.begin(9600);
  pinMode(D4, OUTPUT);
  pinMode(D0, OUTPUT);
}

void loop() {
  float sensor = analogRead(A0);
  Serial.println(sensor);
  delay(800);

  //Convertir valores del sensor a voltaje
  float voltaje = sensor * (5.0 / 1023.0);
  Serial.println(voltaje);
  
  if(voltaje > 1.5){
    digitalWrite(D4, HIGH);
    digitalWrite(D0, LOW);
    Serial.println("LED OFF");
  }
  else{
    digitalWrite(D4, LOW);
    digitalWrite(D0, HIGH);
    Serial.println("LED ON");
  }
    
}
