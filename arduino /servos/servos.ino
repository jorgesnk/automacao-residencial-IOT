#include <Servo.h>  
Servo myservo;  
  #include <ArduinoJson.h>
  #include <ESP8266WiFi.h>
  #include <PubSubClient.h>
  #include <DNSServer.h>
  #include <ESP8266WebServer.h>
  #include <WiFiManager.h> 
  WiFiManager wifiManager;
  WiFiClient espClient;
  PubSubClient client(espClient);
  long lastMsg = 0;
  char msg[50];
  int value = 0;
  const char* mqtt_server = "45.77.112.250";
                
void setup() { 
  myservo.attach(D2);  
  myservo.write(90);
  Serial.begin(9600);
  wifiManager.autoConnect("AutoConnectAP");
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
} 
 
  void callback(char* topic, byte* payload, unsigned int length) {
      String data ;
      String topico= topic;
      for (int i = 0; i < length; i++) {
        data += (char)payload[i];
      }

      Serial.print(topico);
      if(topico=="porta"){
        StaticJsonBuffer<300> JSONBuffer; 
        JsonObject& parsed = JSONBuffer.parseObject(data); 
        const char * comando = parsed["comando"]; 
        Serial.println(comando);
        portas(comando);
      }
  }

  void reconnect() {
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect("teste")) {
        Serial.println("connected");
        client.subscribe("porta");
        client.publish("teste", "hello world");
        // ... and resubscribe
      } 
      else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
    
  }
  void loop() {

    if (!client.connected()) {
      reconnect();
    }
   
    client.loop();
  }


void portas(String comando){
  if(comando=="Abrir"){
    myservo.write(0);
  }
  if(comando=="Fechar"){
    myservo.write(90);
  }
}


