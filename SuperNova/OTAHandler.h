#ifndef OTAHANDLER_H
#define OTAHANDLER_H

#include <ArduinoOTA.h>
#include "arduino_secrets.h" // Incluir para usar los secretos

char otaP[] = SECRET_OTAP; // Contraseña para OTA

void setupOTA() {
  // Iniciar ArduinoOTA con la contraseña
  ArduinoOTA.begin(WiFi.localIP(), "Arduino", otaP, InternalStorage);
  Serial.println("OTA iniciado");
}

void handleOTA() {
  // Manejar las actualizaciones OTA
  ArduinoOTA.poll();
}

#endif // OTAHANDLER_H
