#ifndef GPSHANDLER_H
#define GPSHANDLER_H

#include <Arduino_MKRGPS.h>
#include <time.h>
#include "GlobalVariables.h"

void initGPS() {
  if (!GPS.begin(GPS_MODE_SHIELD)) {
    Serial.println("¡Error al inicializar GPS!");
    while (1); // Detenerse si el GPS no se inicializa
  }
}

int getTimeZoneOffset(float longitude) {
  // Aproximar la zona horaria basada en la longitud
  return round(longitude / 15.0);
}

void updateGPSData() {
  if (GPS.available()) {
    // Leer valores del GPS
    float latitude   = GPS.latitude();
    float longitude  = GPS.longitude();
    float altitude   = GPS.altitude();
    float speed      = GPS.speed();
    int   satellites = GPS.satellites();
    unsigned long epochTime = GPS.getTime();

    // Calcular la zona horaria
    int timeZoneOffset = getTimeZoneOffset(longitude) * 3600; // Convertir horas a segundos

    // Ajustar el epoch time con la zona horaria
    time_t localTime = epochTime + timeZoneOffset;

    // Calcular la fecha y hora a partir del epoch time ajustado
    struct tm* ti;
    ti = gmtime(&localTime);

    int year = ti->tm_year + 1900;
    int month = ti->tm_mon + 1;
    int day = ti->tm_mday;
    int hour = ti->tm_hour;
    int minute = ti->tm_min;
    int second = ti->tm_sec;

    // Aplicar horario de verano (DST) si es necesario
    // Esto es una simplificación. En una implementación real, necesitarías más lógica
    // para determinar si el horario de verano está en efecto basado en la fecha y ubicación.
    bool isDST = true; // Cambiar esta lógica según sea necesario
    if (isDST) {
      hour = (hour + 1) % 24;
    }

    // Formatear la fecha y la hora para mostrar dos dígitos
    char formattedDate[20];
    char formattedTime[20];
    sprintf(formattedDate, "%02d/%02d/%04d", day, month, year);
    sprintf(formattedTime, "%02d:%02d:%02d", hour, minute, second);

    // Almacenar valores del GPS
    gpsData = "Ubicación: " + String(latitude, 7) + ", " + String(longitude, 7) + "\n";
    gpsData += "Altitud: " + String(altitude) + "m\n";
    gpsData += "Velocidad sobre el suelo: " + String(speed) + " km/h\n";
    gpsData += "Número de satélites: " + String(satellites) + "\n";
    gpsData += "Fecha: " + String(formattedDate) + "\n";
    gpsData += "Hora: " + String(formattedTime) + "\n";
    fecha = String(formattedDate) + "\n";
    hora = String(formattedTime) + "\n";
    velocidad = String(speed) + "km/h";
    altitud = "Alt:" + String(altitude) + "m\n";
  }
}

#endif // GPSHANDLER_H
