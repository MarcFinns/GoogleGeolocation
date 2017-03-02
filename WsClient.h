#pragma once

#include "ESP8266WiFi.h"

class WsClient
{
  public:
    WsClient()
    {
      //Serial.println("WsClient constructor");
    };

    ~WsClient()
    {
      //Serial.println("WsClient destructor");
    };

    bool httpsConnect(String hostName, String fingerprint);
    bool httpsGet(String host, String resource);
    bool httpsPost(String host, String url, String postData);
    bool skipResponseHeaders();
    void disconnect();

    WiFiClientSecure client;
};
