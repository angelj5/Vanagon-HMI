#ifndef WEBSERVERHANDLER_H
#define WEBSERVERHANDLER_H

#include <WiFiNINA.h>
#include "GlobalVariables.h" // Incluir el archivo de variables globales

WiFiServer server(80);

void startWebServer() {
  server.begin();
  Serial.println("Servidor web iniciado");
  Serial.print("Usa esta URL para conectarte: http://");
  Serial.println(WiFi.localIP());
}

void handleClient() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Nuevo cliente conectado");
    boolean currentLineIsBlank = true;
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("<html><body>");
          client.println("<h1>Arduino Web Serial Monitor</h1>");
          client.println("<h2>Información de WiFi:</h2>");
          client.println("<pre>" + wifiInfo + "</pre>");
          client.println("<h2>Datos del GPS:</h2>");
          client.println("<pre>" + gpsData + "</pre>");
          client.println("<script>");
          client.println("setTimeout(function() { location.reload(); }, 1000);"); 
          client.println("</script>");
          client.println("</body></html>");
          break;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
          currentLine = "";
        } else if (c != '\r') {
          currentLineIsBlank = false;
          currentLine += c;
        }
      }
    }

    client.stop();
  }
}

#endif // WEBSERVERHANDLER_H
