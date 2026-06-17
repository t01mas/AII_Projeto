# Projeto de Automação II - Leitor RFID via MQTT

Este repositório contém o código de um sistema IoT focado na leitura de tags RFID através um ESP32 e a transmissão desses dados através do protocolo MQTT. O projeto inclui o código em C++ para o microcontrolador, scripts Python de simulação/receção de dados e configurações para a execução de um broker local via Docker.

## Estrutura do Repositório

* **`sketch_jun2a/`**: Código do ESP32 (C++) responsável por ler as tags RFID através do módulo RFID e publicar os dados num broker MQTT.
* **`reciver.py`**: Script Python que subscreve o tópico MQTT e processa as leituras recebidas.
* **`sender.py`**: Script Python para simular a leitura de tags RFID (útil para testes sem hardware).
* **`docker-compose.yml`** e **`conf/`**: Ficheiros para criar um broker MQTT Mosquitto local.
* **`secrets.h`** *(não incluído no controlo de versões)*: Ficheiro no programa principal que contem as credenciais de Wi-Fi e configurações do broker MQTT.
* **`.env`** *(não incluído no controlo de versões)*: Ficheiro com as variáveis de ambiente para os scripts Python.

## Requisitos e Dependências

### Hardware
* ESP32
* Leitor RFID

### Software
* **Docker e Docker Compose** (para o broker local)
* **Python** com as bibliotecas:
    * `paho-mqtt`
    * `python-dotenv`
* **Arduino IDE** com as bibliotecas:
    * `MFRC522`
    * `PubSubClient`
    * `NTPClient`

## Como Executar

1.  **Broker MQTT (Local)**
    Para iniciar o Mosquitto localmente, executa na raiz do projeto:
    ```bash
    docker compose up --build
    ```

2.  **Hardware (ESP32)**
    * Cria um ficheiro `secrets.h` na pasta `sketch_jun2a/` com as seguintes definições:
        ```cpp
        #define SECRET_SSID "a_tua_rede"
        #define SECRET_PASS "a_tua_pass"
        #define SECRET_mqtt_broker "ip_do_broker"
        #define SECRET_mqtt_port 1883
        ```
    * Compila e faz o upload para o ESP32.

3.  **Scripts Python**
    * Instala as dependências Python:
        ```bash
        pip install paho-mqtt python-dotenv
        ```
    * Cria um ficheiro chamado .env na raiz do projeto, com as seguintes definições:
        ```python
        MQTT_BROKER=ip.do.broker
        MQTT_PORT=Porta.do.broker
        ```
    * Para simular envios: `python sender.py`
    * Para receber dados: `python reciver.py`
