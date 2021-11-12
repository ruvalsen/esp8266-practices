#define D4 2
#define D0 16

void setup() {
  pinMode(D4, OUTPUT);
  pinMode(D0, OUTPUT);
}

void loop() {
  digitalWrite(D4, HIGH);   
  delay(1000); 
  digitalWrite(D4, LOW);    
  delay(1000); 

  digitalWrite(D0, HIGH); 
  delay(1000);
  digitalWrite(D0, LOW);    
  delay(1000);   
                          
}
