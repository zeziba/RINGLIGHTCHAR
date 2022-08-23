#ifndef _SERVER_H_
#define _SERVER_H_

#include "env.h"
#include "hex_index.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncWebServer.h>
#include "pins.h"
#include <ArduinoJson.h>

#define PORT 80

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

String outputState(int output);

const uint8_t sizeof_buttons = 1;
const String GUI_buttons(int pin, int pinID)
{
  String rtn = "<h4>Output - GPIO ";
  rtn += String(pinID);
  rtn += "</h4><label class=\"switch\"><input type=\"checkbox\" onclick=\"toggleCheckbox(this)\" id=\"";
  rtn += String(pinID);
  rtn += "\"" + outputState(pin);
  rtn += "><span class=\"slider\"></span></label>";
  return rtn;
};

int input_key_map_size = 1;
const int input_key_map[] = {
  D2
};

// Web Server
static AsyncWebServer server(PORT);

String outputState(int output)
{
  if (digitalRead(output))
  {
    Serial.print("outputState::Toggled " + String(output));
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
      buttons += GUI_buttons(input_key_map[i], i);
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
  for (int i = 0; i < input_key_map_size; i++)
  {
    pinMode(input_key_map[i], OUTPUT);
  }

  // Root page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", data___src_hex_index_h, processor);
  });

  // Update
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request){
    String inputMsg1, inputMsg2;

    // GET input values on <ESP_IP>/update?output=<inputMsg1>&state=<inputMsg2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2))
    {
      inputMsg1 = request->getParam(PARAM_INPUT_1)->value();
      inputMsg2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMsg1.toInt(), inputMsg2.toInt());
    }
    else {
      inputMsg1 = "No message sent.";
      inputMsg2 = inputMsg1;
    }

    char msg1[50], msg2[50];
    inputMsg1.toCharArray(msg1, 50);
    inputMsg2.toCharArray(msg2, 50);
    Serial.printf("GPIO: %s Set as: %s\n", msg1, msg2);
  });

  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}

#endif