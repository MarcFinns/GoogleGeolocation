#pragma once

#include "ESP8266WiFi.h"
#include "JsonListener.h"
#include "Wsclient.h"

class Geocode : public JsonListener, public WsClient
{
  public:
    Geocode() {
      //Serial.println("Geocode constructor");
    };
    ~Geocode() {
      //Serial.println("Geocode destructor");
    };

    bool acquire(double latitude, double longitude);
    bool isValid();
    String getStreet();
    String getStreetNumber();
    String getPostCode();
    String getSubLocality();
    String getTown();
    String getCountry();
    String getFormattedAddress();

    virtual void whitespace(char c);
    virtual void startDocument();
    virtual void key(String key);
    virtual void value(String value);
    virtual void endArray();
    virtual void endObject();
    virtual void endDocument();
    virtual void startArray();
    virtual void startObject();

  private:
    bool valid = false;
    String currentKey;
    String savedValue;
    String street;
    String streetNumber;
    String postCode;
    String subLocality;
    String town;
    String country;
    String formattedAddress;


};
