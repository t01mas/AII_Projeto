import json
import random
import time
import paho.mqtt.client as mqtt


client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, "SimuladorRFID")

print("A ligar ao Broker MQTT local...")
client.connect("ims.mec.ua.pt", 82)

topico = "aii/projeto/rfid"
print(f"A publicar em '{topico}'...\n")

try:
    while True:
        # Gera uma leitura simulada
        tag_uid = "A1B2C3D4"
        time_atual = int(time.time())

        # Estrutura JSON adaptada ao RFID
        dados_sensor = {
            "dispositivo": "esp32_rfid_1",
            "uid": tag_uid,
            "status": "lido",
            "timestamp": time_atual,
        }

        mensagem_json = json.dumps(dados_sensor)

        client.publish(topico, mensagem_json)
        print(f"[ENVIADO] {mensagem_json}")

        # Simula uma passagem de cartão a cada 3 a 8 segundos
        time.sleep(random.randint(3, 8))

except KeyboardInterrupt:
    print("\nSimulação interrompida pelo utilizador.")

finally:
    client.disconnect()
    print("Desligado do Broker MQTT.")

