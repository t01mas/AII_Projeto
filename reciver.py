import paho.mqtt.client as mqtt
import json

FICHEIRO_TESTE = "logs_rfid.jsonl"
TOPICO = "aii/projeto/rfid"


def on_message(client, userdata, message):
    conteudo = message.payload.decode("utf-8")
    print(f"[RECEBIDO] Tópico: {message.topic} | Mensagem: {conteudo}")

    # Processamento do JSON
    try:
        dados_json = json.loads(conteudo)
        uid_lido = dados_json.get("uid", "Desconhecido")
        print(f" -> TAG detetada com sucesso: {uid_lido}\n")

        # with open(FICHEIRO_TESTE, "a", encoding="utf-8") as f:
        #     json.dump(dados_json, f)
        #     f.write("\n")

    except json.JSONDecodeError:
        print("[AVISO] A mensagem recebida não é um JSON válido. Ignorada.")
    except Exception as e:
        print(f"[ERRO] Ocorreu um problema: {e}")


def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print("Ligado ao Broker MQTT com sucesso!")
        client.subscribe(TOPICO)
        print(f"À escuta no tópico '{TOPICO}'...\n")
    else:
        print(f"Falha na ligação. Código de erro: {reason_code}")


client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, "ClienteReceptorAII")

client.on_connect = on_connect
client.on_message = on_message

print("A tentar ligar ao broker...")
client.connect("ims.mec.ua.pt", 82)

try:
    client.loop_forever()
except KeyboardInterrupt:
    print("\nA desligar o recetor...")
    client.disconnect()
