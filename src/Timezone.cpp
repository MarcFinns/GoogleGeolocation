#include "ESP8266WiFi.h"
#include <JsonStreamingParser.h>
#include "Timezone.h"
#include "UserConfig.h"

//extern NTPClient NTP;


bool Timezone::acquire(double latitude, double longitude, String timestamp)
{
  /**********************************************************
     Step 2 - Timezone
     Get Timezone from latitude and longitude
   **********************************************************/

  //Connect to the client and make the api call
  if (httpsConnect(maps_Host, maps_fingerprint))
  {
    // Serial.println(timeZone_url + String(latitude, 8) + "," + String(longitude, 8) + "&timestamp=" + timestamp + "&key=" + googleApiKey);

    // Concatenate url and key
    if (httpsGet(maps_Host, timeZone_url + String(latitude, 6) + "," + String(longitude, 6) + "&timestamp=" + timestamp + "&key=" + googleApiKey) && skipResponseHeaders())
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
    //Serial.println("Could not connect");
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
  if (currentKey == "dstOffset")
  {
    dstOffset = value.toInt();
  }
  else if (currentKey == "rawOffset")
  {
    rawOffset = value.toInt();
  }
  else if (currentKey == "timeZoneId")
  {
    timeZoneId = value;
  }
  else if (currentKey == "timeZoneName")
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
