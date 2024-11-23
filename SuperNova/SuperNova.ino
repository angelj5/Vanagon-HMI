#include <Arduino.h>
#include "WiFiHandler.h"
#include "OTAHandler.h"
#include "GPSHandler.h"
#include "WebServerHandler.h"
#include "DisplayHandler.h"
#include "GlobalVariables.h"
#include <TaskScheduler.h>

// Declarar el scheduler y la tarea de refresco de pantalla
Scheduler runner;
void refreshDisplayTaskCallback();
Task refreshDisplayTask(1000, TASK_FOREVER, &refreshDisplayTaskCallback);

// Definición de variables globales
String wifiInfo = "";
String gpsData = "";
String velocidad = "";
String fecha = "";
String hora = "";
String altitud = "";

void setup() {
  Serial.begin(115200);
    
  initGPS();
  setupDisplay();
  connectToWiFi();
  setupOTA();
  startWebServer();

  runner.addTask(refreshDisplayTask);
  refreshDisplayTask.enable();
}

void loop() {
  handleOTA();
  handleClient();
  updateGPSData();
  runner.execute();
}

void refreshDisplayTaskCallback() {
  updateDisplay(velocidad, fecha, hora, altitud);
}
