#ifndef _SERVER_H_
#define _SERVER_H_

#include "env.h"
#include "index.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>

#define PORT 80

String loadFile(char *file)
{
  return "";
}

// Web Server
static ESP8266WebServer server(PORT);
// Serverable Web page
void handleRoot()
{
  server.send(200, "text/html", index_html);
}

void handlePlain()
{
  if (server.method() != HTTP_POST)
  {
    server.send(405, "text/plain", "Method Not Allowed");
  }
  else
  {
    server.send(200, "text/plain", "POST body was:\n" + server.arg("plain"));
  }
}

void handleForm()
{
  if (server.method() != HTTP_POST)
  {
    server.send(405, "text/plain", "Method Not Allowed");
  }
  else
  {
    String message = "POST form was:\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(200, "text/plain", message);
  }
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

static void wifiServerStart()
{
  Serial.printf("Connecting to %s ", SSID_M);
  WiFi.begin(SSID_M, PSWD_M);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Serial.println();

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/postplain/", handlePlain);
  server.on("/postform/", handleForm);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}

#endif