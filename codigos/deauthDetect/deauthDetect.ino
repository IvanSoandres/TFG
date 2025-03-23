// This software is licensed under the MIT License.
// See the license file for details.
// For more details visit github.com/spacehuhn/DeauthDetector

// include necessary libraries
#include <ESP8266WiFi.h>

// include ESP8266 Non-OS SDK functions
extern "C" {
#include "user_interface.h"
}

// ===== SETTINGS ===== //
#define LED 2              /* LED pin (2=built-in LED) */
#define LED_INVERT true    /* Invert HIGH/LOW for LED */
#define SERIAL_BAUD 9600   /* Baudrate for serial communication */
#define CH_TIME 140        /* Scan time (in ms) per channel */
#define PKT_RATE 5         /* Min. packets before it gets recognized as an attack */
#define PKT_TIME 1         /* Min. interval (CH_TIME*CH_RANGE) before it gets recognized as an attack */
#define MEASURE_TIMING true // Flag to enable timing measurements

// Channels to scan on (US=1-11, EU=1-13, JAP=1-14)
const short channels[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13/*,14*/ };

// ===== Runtime variables ===== //
int ch_index { 0 };               // Current index of channel array
int packet_rate { 0 };            // Deauth packet counter (resets with each update)
int attack_counter { 0 };         // Attack counter
unsigned long update_time { 0 };  // Last update time
unsigned long ch_time { 0 };      // Last channel hop time
unsigned long first_detection_time { 0 }; // Time of first deauth packet detection
unsigned long attack_detected_time { 0 }; // Time when attack is detected
bool attack_detected { false };   // Flag to indicate if an attack is detected

// ===== Sniffer function ===== //
void sniffer(uint8_t *buf, uint16_t len) {
  if (!buf || len < 28) return; // Drop packets without MAC header

  byte pkt_type = buf[12]; // second half of frame control field

  // If captured packet is a deauthentication or disassociation frame
  if (pkt_type == 0xA0 || pkt_type == 0xC0) {
    ++packet_rate;

    // Record the time of the first deauth packet detection
    if (first_detection_time == 0) {
      first_detection_time = millis();
      Serial.print("First deauth packet detected at: ");
      Serial.println(first_detection_time);
    }
  }
}

// ===== Attack detection functions ===== //
void attack_started() {
  digitalWrite(LED, !LED_INVERT); // turn LED on
  Serial.println("ATTACK DETECTED");
  attack_detected_time = millis();
  attack_detected = true;

  if (MEASURE_TIMING) {
    unsigned long threshold_time = attack_detected_time - first_detection_time;
    Serial.print("Threshold time: ");
    Serial.println(threshold_time);
    Serial.print("First detection time: ");
    Serial.println(first_detection_time);
    Serial.print("Attack detected time: ");
    Serial.println(attack_detected_time);
  }
}

void attack_stopped() {
  digitalWrite(LED, LED_INVERT); // turn LED off
  Serial.println("ATTACK STOPPED");
  attack_detected = false;
  first_detection_time = 0; // Reiniciar el tiempo de la primera detección
  attack_detected_time = 0; // Reiniciar el tiempo de detección del ataque
  Serial.println("Timers reset.");
}

// ===== Setup ===== //
void setup() {
  Serial.begin(SERIAL_BAUD); // Start serial communication

  pinMode(LED, OUTPUT); // Enable LED pin
  digitalWrite(LED, LED_INVERT);

  WiFi.disconnect();                   // Disconnect from any saved or active WiFi connections
  wifi_set_opmode(STATION_MODE);       // Set device to client/station mode
  wifi_set_promiscuous_rx_cb(sniffer); // Set sniffer function
  wifi_set_channel(channels[0]);        // Set channel
  wifi_promiscuous_enable(true);       // Enable sniffer

  Serial.println("Started IDS\\o/");
}

// ===== Loop ===== //
void loop() {
  unsigned long current_time = millis(); // Get current time (in ms)

  // Update each second (or scan-time-per-channel * channel-range)
  if (current_time - update_time >= (sizeof(channels) * CH_TIME)) {
    update_time = current_time; // Update time variable

    // When detected deauth packets exceed the minimum allowed number
    if (packet_rate >= PKT_RATE) {
      ++attack_counter; // Increment attack counter
    } else {
      if (attack_counter >= PKT_TIME) attack_stopped();
      attack_counter = 0; // Reset attack counter
    }

    // When attack exceeds minimum allowed time
    if (attack_counter == PKT_TIME) {
      attack_started();
    }

    Serial.print("Packets/s: ");
    Serial.println(packet_rate);

    packet_rate = 0; // Reset packet rate
  }

  // Channel hopping
  if (sizeof(channels) > 1 && current_time - ch_time >= CH_TIME) {
    ch_time = current_time; // Update time variable

    // Get next channel
    ch_index = (ch_index + 1) % (sizeof(channels) / sizeof(channels[0]));
    short ch = channels[ch_index];

    // Set channel
    wifi_set_channel(ch);
  }

  // Measure total detection time if an attack is detected
  if (MEASURE_TIMING && attack_detected) {
    unsigned long total_detection_time = millis() - first_detection_time;
    Serial.print("Total detection time: ");
    Serial.println(total_detection_time);
  }
}
