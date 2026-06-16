#include <SPI.h>
#include <MFRC522.h>
#include <time.h>
#include <WiFiUdp.h> 
#include <WiFi.h>
#include <NTPClient.h>
#include "secrets.h"
#include <PubSubClient.h>

#define SS_PIN  5
#define RST_PIN 0

unsigned long timeStamp;

MFRC522 mfrc522(SS_PIN, RST_PIN);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variáveis GLOBAIS para o MQTT
WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  while (!client.connected()) {
    Serial.print("A tentar conectar ao MQTT...");
    if (client.connect("ESP32_RFID_Client")) {
      Serial.println("Conectado ao servidor MQTT!");
    } else {
      Serial.print("Erro a conectar: ");
      Serial.print(client.state());
      Serial.println(" Nova tentativa em 5s...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  Serial.print("A Ligar ao WiFi...");
  WiFi.begin(SECRET_SSID, SECRET_PASS); 

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConectado ao WiFi com sucesso!");
  Serial.println(WiFi.localIP()); 

  timeClient.begin();
  timeClient.setTimeOffset(3600);

  SPI.begin();
  mfrc522.PCD_Init();

  client.setServer(SECRET_mqtt_broker, SECRET_mqtt_port);

  Serial.println("Aproxima uma tag do leitor RFID.");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 

  timeClient.update();

 
  // Verifica se há uma nova tag próxima ao leitor
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Ler os dados da tag
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  timeStamp = timeClient.getEpochTime();
  Serial.print("[");
  Serial.print(timeStamp);
  Serial.print("] Tag detetada! UID: ");

  String tagUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagUID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    tagUID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  tagUID.toUpperCase();
  String finalUID = tagUID.substring(1);
  Serial.println(finalUID);

  String payload = "{\"uid\": \"" + finalUID + "\", \"timestamp\": \"" + String(timeStamp) + "\"}";
  
  Serial.print("A enviar JSON: ");
  Serial.println(payload);

  client.publish("aii/projeto/rfid", payload.c_str());

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1(); 
}
