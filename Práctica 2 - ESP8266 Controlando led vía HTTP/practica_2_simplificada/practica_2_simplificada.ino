#include <ESP8266WiFi.h>

const char* ssid     = "";
const char* password = "";
String currentState = "off";
const int D0 = 16;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);

  // Conexión a la red Wifi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  //Imprimir la ip del servidor
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  //Saber si un cliente se conectó
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("new client"));

  //Esperar 5 segundos para la petición
  client.setTimeout(5000); 

  //Leer primer línea de la petición
  String req = client.readStringUntil('\r');
  Serial.println(F("----------------------------"));
  Serial.print(F("request: "));
  Serial.println(req);

  //Encender/Apagar el led (D0)
  if (req.indexOf("GET /16/on") >= 0) {
    Serial.println("GPIO 16 on");
    currentState = "on";
    digitalWrite(D0, HIGH);
  } else if (req.indexOf("GET /16/off") >= 0) {
    Serial.println("GPIO 16 off");
    currentState = "off";
    digitalWrite(D0, LOW);
  } 

  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }
            
  //Mandar el HTML
  client.println("<!DOCTYPE html><html lang='es'>");
  client.println("<meta charset=\"UTF-8\">");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  
  //Estilos para el documento
  client.println("<style>html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #00c652; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #bdbdbd;}</style></head>");
  
  //Título página
  client.println("<body><h1>Práctica 2 - ESP8266 Web Server</h1>");
  
  //Mostrar el estado actual del led
  client.println("<p>Pin 16 - " + currentState + "</p>");
  
  //Si el led está apagado mostrar el botón de encender       
  if (currentState=="off") {
    client.println("<p><a href=\"/16/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    //Si no mostrar el botón de apagar
    client.println("<p><a href=\"/16/off\"><button class=\"button button2\">OFF</button></a></p>");
  } 
     
  client.println("</body></html>");
  
  //Terminar la respuesta HTTP con un salto de línea
  client.println();

  Serial.println(F("Disconnecting from client"));
}
