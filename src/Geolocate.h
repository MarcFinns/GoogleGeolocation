#pragma once

#include "ESP8266WiFi.h"
#include "JsonListener.h"
#include "Wsclient.h"

class Geolocate : public JsonListener, public WsClient
{
  public:
    Geolocate() {
      //Serial.println("Geolocate constructor");
    };
    ~Geolocate() {
      //Serial.println("Geolocate destructor");
    };

    bool acquire();
    bool isValid();
    virtual void whitespace(char c);
    virtual void startDocument();
    virtual void key(String key);
    virtual void value(String value);
    virtual void endArray();
    virtual void endObject();
    virtual void endDocument();
    virtual void startArray();
    virtual void startObject();
    double getLatitude();
    double getLongitude();

  private:
    bool valid = false;
    double latitude    = 0.0;
    double longitude   = 0.0;
    String currentKey;
    String currentParent;
};
