#ifndef _SERVER_H_
#define _SERVER_H_

#include "env.h"
#include "hex_index.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "pins.h"
#include <ArduinoJson.h>

#define PORT 80

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

String outputState(int output);

const uint8_t sizeof_buttons = 1;
const String GUI_buttons[] PROGMEM = {
  "<h4>Output - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(D2) + "><span class=\"slider\"></span></label>"
};

// Web Server
static AsyncWebServer server(PORT);

String outputState(int output)
{
  if (digitalRead(output))
  {
    Serial.print("outputState::Toggled " + output);
    Serial.println(" pin.");
    return "checked";
  }
  else
  {
    return "";
  }
}

String processor(const String& var)
{
  // Serial.println(var);
  if (var == "BUTTONPLACEHOLDER")
  {
    String buttons = "";
    
    if (sizeof_buttons == 0)
    {
      return buttons;
    }
    
    for (int i = 0; i < sizeof_buttons; i++)
    {
      buttons += GUI_buttons[i];
    }
    return buttons;
  }
  return String();
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

  // Start up onBoard LED for status light
  pinMode(D4, OUTPUT);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", data___src_hex_index_h, processor);
  });
  // server.on("/postplain/", handlePlain);
  // server.on("/postform/", handleForm);
  // server.onNotFound(handleNotFound);

  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}

#endif