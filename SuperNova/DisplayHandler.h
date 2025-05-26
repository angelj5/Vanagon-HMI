#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_CS        6
#define TFT_RST       5
#define TFT_DC        7

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Variables para almacenar el estado anterior
String previousVelocidad = "";
String previousVelocidadMph = "";
String previousHora = "";
String previousFecha = "";
String previousAltitud = "";

void setupDisplay() {
  tft.init(170, 320);           // Initialize display
  tft.setRotation(1);           // Set rotation if needed
  tft.fillScreen(ST77XX_BLACK); // Clear the screen 
}

void updateDisplay(const String& velocidad, const String& fecha, const String& hora, const String& altitud) {
  tft.setTextColor(ST77XX_WHITE); // Set text color
  tft.setTextWrap(false);         // No text wrap

  // Convertir la velocidad a un número entero
  int velocidadEnteraMph = (velocidad.toFloat() * 0.621371); // Convertir de km/h a mph y eliminar los decimales

  // Convertir el número entero de vuelta a String para mostrarlo
  String velocidadStrMph = String(velocidadEnteraMph);

  // Actualizar la velocidad dígito por dígito
  tft.setTextSize(2);
  for (int i = 0; i < velocidad.length(); i++) {
    if (i >= previousVelocidad.length() || velocidad[i] != previousVelocidad[i]) {
      tft.setCursor(180 + i * 12, 80);  // Ajustar posición según el tamaño del texto
      tft.fillRect(180 + i * 12, 80, 12, 14, ST77XX_BLACK); // Borrar solo el dígito que cambió
      tft.print(velocidad[i]);
    }
  }

  previousVelocidad = velocidad; 

  // Calcular la posición inicial del cursor para alinear a la derecha
  int cursorX = 160 - (velocidadStrMph.length() * 60); // 60 es el ancho de cada dígito
  
  // Actualizar la velocidad mph dígito por dígito
  tft.setTextSize(10);
  
  // Borrar ambos dígitos si se pasa de dos dígitos a un dígito (por ejemplo, de 10 a 9 mph)
if (previousVelocidadMph.length() > velocidadStrMph.length()) {
  tft.fillRect(cursorX - 60, 10, 120, 70, ST77XX_BLACK); // Borrar el área completa de ambos dígitos
}
  for (int i = 0; i < velocidadStrMph.length(); i++) {
    if (i >= previousVelocidadMph.length() || velocidadStrMph[i] != previousVelocidadMph[i]) {
      tft.setCursor(cursorX + i * 60, 10);  // Ajustar posición según el tamaño del texto
      tft.fillRect(cursorX + i * 60, 10, 60, 70, ST77XX_BLACK); // Borrar solo el dígito que cambió
      tft.print(velocidadStrMph[i]);
    }
  }

  previousVelocidadMph = velocidadStrMph; 

  tft.setTextSize(3);
  tft.setCursor(70, 90);
  tft.print("mph");
  
  // Actualizar la hora dígito por dígito
  tft.setTextSize(3);
  for (int i = 0; i < hora.length(); i++) {
    if (i >= previousHora.length() || hora[i] != previousHora[i]) {
      tft.setCursor(180 + i * 18, 10);  // Ajustar posición según el tamaño del texto
      tft.fillRect(180 + i * 18, 10, 18, 21, ST77XX_BLACK); // Borrar solo el dígito que cambió
      tft.print(hora[i]);
    }
  }

  previousHora = hora;

  // Actualizar la fecha si ha cambiado
  if (fecha != previousFecha) {
    tft.setTextSize(2);
    tft.setCursor(180, 40);
    tft.fillRect(180, 40, 180, 14, ST77XX_BLACK); // Borrar la línea de la fecha
    tft.print(fecha);
    previousFecha = fecha;
  }

  // Actualizar la altitud dígito por dígito
  tft.setTextSize(2);
  for (int i = 0; i < altitud.length(); i++) {
    if (i >= previousAltitud.length() || altitud[i] != previousAltitud[i]) {
      tft.setCursor(180 + i * 12, 60);  // Ajustar posición según donde quieras mostrar la altitud
      tft.fillRect(180 + i * 12, 60, 12, 14, ST77XX_BLACK); // Borrar solo el dígito que cambió
      tft.print(altitud[i]);
    }
  }

  previousAltitud = altitud;
}

#endif // DISPLAYHANDLER_H
