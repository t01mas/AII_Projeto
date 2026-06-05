#include <SPI.h>
#include <MFRC522.h>
#include <time.h>
#include <WiFiUdp.h> 
#include <WiFi.h>
#include <NTPClient.h>
#include "secrets.h"

#define SS_PIN  5
#define RST_PIN 0

// Variavel para armazenar a data e hora
String timeStamp;

MFRC522 mfrc522(SS_PIN, RST_PIN);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup() {
  Serial.begin(115200);

  Serial.print("A Ligar ao WiFi...");

  //trocar para os dados do Wifi disponivel
  WiFi.begin(SECRET_SSID, SECRET_PASS); 

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);Serial.print(".");
  }

  Serial.println("Conectado ao WiFi com sucesso!");
  Serial.println(WiFi.localIP()); 

  timeClient.begin();
  timeClient.setTimeOffset(3600);

  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Aproxima uma tag do leitor RFID.");
}

void loop() {
  // Verifica se há uma nova tag próxima ao leitor
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Ler os dados da tag
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Tag detetada! UID:");

  String tagUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagUID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    tagUID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  tagUID.toUpperCase();
  // Imprime o UID a partir do primeiro caracter útil (ignora o primeiro espaço)
  Serial.println(tagUID.substring(1));

  // Instrui a tag a parar de comunicar para não gerar leituras contínuas da mesma aproximação
  mfrc522.PICC_HaltA();
}
