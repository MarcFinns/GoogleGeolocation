#pragma once

#include "ESP8266WiFi.h"
#include "JsonListener.h"
#include "Wsclient.h"

class Timezone : public JsonListener, public WsClient
{
  public:
    Timezone() {
      //Serial.println("Timezone constructor");
    };
    ~Timezone() {
      //Serial.println("Timezone destructor");
    };

    bool acquire(double latitude, double longitude, String timestamp);
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
    String getTimeZoneId();
    String getTimeZoneName();
    int getDstOffset();
    int getRawOffset();

  private:
    bool valid = false;
    String currentKey;
    String currentParent;
    int dstOffset;
    int rawOffset;
    String timeZoneId;
    String timeZoneName;
};
