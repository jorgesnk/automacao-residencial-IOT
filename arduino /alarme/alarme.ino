
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
  const char* mqtt_server = "192.168.0.23";

  int sensor=D2;
  int buzer= D1;
  bool ativar=false;  
void setup() { 

  pinMode(sensor,INPUT);
  pinMode(buzer,OUTPUT);
  
  digitalWrite(buzer,LOW);
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
        
      }
  }

  void reconnect() {
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect("teste")) {
        Serial.println("connected");
        client.subscribe("alarme");
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
    alarme();
  }

  void alarme(){
    
    int valorSensor=digitalRead(sensor);
    Serial.println(valorSensor);
    if(valorSensor==1){
       char* valorJson="{alarme:'tocar'}";
       ativar=true;
       client.publish("alarme",valorJson);
    }

    if(ativar==true){
      digitalWrite(buzer,HIGH);
    }
    delay(2000);
    digitalWrite(buzer,LOW);
 
    
  }



