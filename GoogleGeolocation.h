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
