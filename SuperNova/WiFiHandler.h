#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <WiFiNINA.h>
#include "arduino_secrets.h" // Incluir para usar los secretos
#include "GlobalVariables.h" // Incluir el archivo de variables globales

// Configuración para la red Wi-Fi objetivo
const char* targetSSID = "Chulla Vida 2.4"; // Cambia esto por tu red objetivo
int rssiThreshold = -70; // Umbral de señal predeterminado

bool targetNetworkFound = false;
bool signalIsStrong = false;

void checkWiFiNetworks() {
  int n = WiFi.scanNetworks(); // Escanear redes disponibles
  targetNetworkFound = false;
  signalIsStrong = false;
  
  for (int i = 0; i < n; i++) {
    String ssid = WiFi.SSID(i);
    int rssi = WiFi.RSSI(i);

    // Verificar si la red objetivo está presente y comparar la señal RSSI
    if (ssid == targetSSID) {
      targetNetworkFound = true;
      Serial.print("Red objetivo encontrada: ");
      Serial.print(ssid);
      Serial.print(" con RSSI: ");
      Serial.println(rssi);

      if (rssi > rssiThreshold) {
        signalIsStrong = true;
        Serial.println("La señal es suficientemente fuerte.");
      } else {
        Serial.println("La señal es demasiado débil.");
      }
    }
  }
  
  if (!targetNetworkFound) {
    Serial.println("No se encontró la red objetivo.");
  }
}

void connectToWiFi() {
  // Escanear redes Wi-Fi antes de intentar conectarse
  checkWiFiNetworks();

  // Intentar conectar solo si la red objetivo fue encontrada y la señal es fuerte
  if (targetNetworkFound && signalIsStrong) {
    Serial.print("Conectando a ");
    Serial.println(SECRET_SSID);
    WiFi.begin(SECRET_SSID, SECRET_PASS);

    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("Conectado a la red Wi-Fi");
    Serial.println("Dirección IP: ");
    Serial.println(WiFi.localIP());

    wifiInfo = "Conectado a: " + String(SECRET_SSID) + "\n";
    wifiInfo += "Dirección IP: " + WiFi.localIP().toString() + "\n";
  } else {
    Serial.println("No se intentó conectar debido a la falta de una red objetivo con señal suficiente.");
  }
}

#endif // WIFIHANDLER_H
