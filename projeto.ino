// From https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide

#include <WiFi.h>
#include "PubSubClient.h"

const char* ssid = "ifsp-ibge-1";
const char* password = "ifspcatanduva";
const char* mqttServer = "broker.hivemq.com";
int port = 1883;
String stMac;
char mac[50];
char clientId[50];

WiFiClient espClient;
PubSubClient client(espClient);

const int ledSala = 21;
const int ledQuarto = 22;
const int ledCozinha = 19;
const int ledBanheiro = 18;


void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  wifiConnect();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
  stMac = WiFi.macAddress();
  stMac.replace(":", "_");
  Serial.println(stMac);
  
  client.setServer(mqttServer, port);
  client.setCallback(callback);
  
  pinMode(ledSala, OUTPUT);
  pinMode(ledQuarto, OUTPUT);
  pinMode(ledCozinha, OUTPUT);
  pinMode(ledBanheiro, OUTPUT);
}

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void mqttReconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    long r = random(1000);
    sprintf(clientId, "clientId-%ld", r);
    if (client.connect(clientId)) {
      Serial.print(clientId);
      Serial.println(" connected");
     
      client.subscribe("Miguel/quarto");
      client.subscribe("Miguel/sala");
      client.subscribe("Miguel/cozinha");
      client.subscribe("Miguel/banheiro");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Mensagem recebida no topico: ");
  Serial.print(topic);
  Serial.print(". Messagem: ");
  String stMessage;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    stMessage += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "Miguel/sala") {
    Serial.print("Saida alterada para ");
    if(stMessage == "on"){
      Serial.println("on");
      digitalWrite(ledSala, HIGH);
    }
    else if(stMessage == "off"){
      Serial.println("off");
      digitalWrite(ledSala, LOW);
    }
  }


  if (String(topic) == "Miguel/cozinha") {
    Serial.print("Saida alterada para ");
    if(stMessage == "on"){
      Serial.println("on");
      digitalWrite(ledCozinha, HIGH);
    }
    else if(stMessage == "off"){
      Serial.println("off");
      digitalWrite(ledCozinha, LOW);
    }
  }
  
  if (String(topic) == "Miguel/quarto") {
    Serial.print("Saida alterada para ");
    if(stMessage == "on"){
      Serial.println("on");
      digitalWrite(ledQuarto, HIGH);
    }
    else if(stMessage == "off"){
      Serial.println("off");
      digitalWrite(ledQuarto, LOW);
    }
  }

  
  if (String(topic) == "Miguel/banheiro") {
    Serial.print("Saida alterada para ");
    if(stMessage == "on"){
      Serial.println("on");
      digitalWrite(ledBanheiro, HIGH);
    }
    else if(stMessage == "off"){
      Serial.println("off");
      digitalWrite(ledBanheiro, LOW);
    }
  }
  
}

void loop() {
  delay(10);
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();
}
