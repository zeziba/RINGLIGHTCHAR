#ifndef _SERVER_H_
#define _SERVER_H_

#include "env.h"
#include "index.h"
#include <ESP8266WiFi.h>

#define PORT 80


// Web Server
extern WiFiServer server(PORT);
// Serverable Web page
String prepareHtmlPage()
{
    return index_html;
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

    WiFi.begin("network-name", "pass-to-network");

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    server.begin();
    Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}

#endif