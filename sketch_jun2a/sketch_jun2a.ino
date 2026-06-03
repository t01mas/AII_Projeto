#include <SPI.h>
#include <MFRC522.h>
#include <time.h>
#include <WiFiClient.h> 

#define SS_PIN  5
#define RST_PIN 0

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);

  Serial.print("Conectando ao WiFi...");

  //trocar para os dados do Wifi disponivel
  WiFi.begin("", ""); 

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);Serial.print(".");
  }

  Serial.println("Conectado ao WiFi com sucesso!");
  Serial.println(WiFi.localIP()); 

  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Aproxima uma tag do leitor RFID.");

}

void loop() {
  // Verifica se há uma nova tag próxima ao leitor
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Tenta ler os dados da tag
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Tag detetada! UID:");

  String tagUID = "";
  // O UID normalmente tem 4 bytes, este loop extrai-os
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
