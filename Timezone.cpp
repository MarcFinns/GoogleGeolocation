#include "ESP8266WiFi.h"
#include <JsonStreamingParser.h>
#include "GoogleGeolocation.h"
#include "UserConfig.h"

//extern NTPClient NTP;


bool Timezone::acquire(double latitude, double longitude, String timestamp)
{
  /**********************************************************
     Step 2 - Timezone
     Get Timezone from latitude and longitude
   **********************************************************/

  //Connect to the client and make the api call
  if (httpsConnect(FPSTR(maps_Host), FPSTR(maps_fingerprint)))
  {
    // Concatenate url and key
    String url = FPSTR(timeZone_url);
    url += String(latitude, 6);
    url += F(",") ;
    url += String(longitude, 6) ;
    url += F("&timestamp=") ;
    url += timestamp;
    url += F("&key=") ;
    url += FPSTR(googleApiKey);

    if (httpsGet(FPSTR(maps_Host), url) && skipResponseHeaders())
    {
      JsonStreamingParser parser;
      parser.setListener(this);
      char c;
      int size = 0;

      while ((size = client.available()) > 0)
      {
        c = client.read();
        parser.parse(c);
      }
    }
    else
    {
      // Get failed
      // Serial.println("get failed");
      return false;
    }
  }
  else
  {
    // Could not connect
    // Serial.println("Could not connect");
    return false;
  }

  disconnect();

  valid = true;

  return true;
}

bool Timezone::isValid()
{
  return valid;
}

void Timezone::whitespace(char c) {
  // Serial.println("whitespace");
}

void Timezone::startDocument() {
  // Serial.println("start document");
}

void Timezone::key(String key) {
  // Serial.println("key: " + key);
  currentKey = String(key);
}

void Timezone::value(String value)
{
  // Serial.println("value: " + value);
  if (currentKey == F("dstOffset"))
  {
    dstOffset = value.toInt();
  }
  else if (currentKey == F("rawOffset"))
  {
    rawOffset = value.toInt();
  }
  else if (currentKey == F("timeZoneId"))
  {
    timeZoneId = value;
  }
  else if (currentKey == F("timeZoneName"))
  {
    timeZoneName = value;
  }
}

void Timezone::endArray() {
  // Serial.println("end array. ");
}

void Timezone::endObject() {
  // Serial.println("end object. ");
}

void Timezone::endDocument() {
  // Serial.println("end document. ");
}

void Timezone::startArray() {
  // Serial.println("start array. ");
}

void Timezone::startObject() {
  // Serial.println("start object. ");
}

String Timezone::getTimeZoneId()
{
  return timeZoneId;
}

String Timezone::getTimeZoneName()
{
  return timeZoneName;
}

int Timezone::getDstOffset()
{
  return dstOffset;
}

int Timezone::getRawOffset()
{
  return rawOffset;
}
