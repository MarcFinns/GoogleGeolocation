//#include <ESP8266HTTPclient.h>
#include <ArduinoJson.h>
#include "ESP8266WiFi.h"
#include <NtpClientLib.h>         // https://github.com/gmag11/NtpClient
#include <JsonStreamingParser.h>
#include "UserConfig.h"
#include "GoogleGeolocation.h"

/**********************************************************
   Step 3 - Geocoding
   Get Location name from latitude and longitude
 **********************************************************/
bool Geocode::acquire(double latitude, double longitude)
{
  //Connect to the client and make the api call
  if (httpsConnect(FPSTR(maps_Host), FPSTR(maps_fingerprint)))
  {
    // Serial.println(geocoding_url + String(latitude, 8) + "," + String(longitude, 8) + "&key="  + googleApiKey);

    // Concatenate url and key
    String url = FPSTR(geocoding_url);
    url += String(latitude, 8);
    url += F(",");
    url += String(longitude, 8);
    url += F("&key=" );
    url += FPSTR(googleApiKey);

    if (httpsGet(FPSTR(maps_Host), url) && skipResponseHeaders())
    {
      // Invalidate current values
      street = FPSTR(empty);
      streetNumber = FPSTR(empty);
      postCode = FPSTR(empty);
      subLocality = FPSTR(empty);
      town = FPSTR(empty);
      country = FPSTR(empty);
      formattedAddress = FPSTR(empty);

      while (client.available())
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
    }
    else
      // Get failed
      return false;
  }
  else
    // Could not connect
    return false;

  disconnect();

  valid = true;

  return true;
}

bool Geocode::isValid()
{
  return valid;
}

void Geocode::whitespace(char c) {
  // Serial.println("whitespace");
}

void Geocode::startDocument() {
  //   // Serial.println("start document");
}

void Geocode::key(String key)
{
  // Serial.println("key: " + key);
  currentKey = key;
}

void Geocode::value(String value)
{
  //   // Serial.println("value: " + value);
  if (currentKey == F("long_name"))
    savedValue = value;
  else if (currentKey == F("types"))
  {
    if (value == F("route"))
    {
      if (street == "")
        street = savedValue;
    }
    else if (value == F("street_number"))
    {
      if (streetNumber == "")
        streetNumber = savedValue;
    }
    else if (value == F("postal_code"))
    {
      if (postCode == "")
        postCode = savedValue;
    }
    else if (value == F("sublocality"))
    {
      if (country == "")
        subLocality = savedValue;
    }
    else if (value == F("locality"))
    {
      if (town == "")
        town = savedValue;
    }
    else if (value == F("country"))
    {
      if (country == "")
        country = savedValue;
    }
    else if (value == F("formatted_address"))
    {
      if (formattedAddress == "")
        formattedAddress = savedValue;
    }
  }
}


void Geocode::endArray() {
  // Serial.println("end array. ");
}

void Geocode::endObject() {
  //   // Serial.println("end object. ");
}

void Geocode::endDocument() {
  //   // Serial.println("end document. ");
}

void Geocode::startArray() {
  //   // Serial.println("start array. ");
}

void Geocode::startObject() {
  //   // Serial.println("start object. ");
}

String Geocode::getStreet()
{
  return street;
}

String Geocode::getStreetNumber()
{
  return streetNumber;
}

String Geocode::getPostCode()
{
  return postCode;
}

String Geocode::getSubLocality()
{
  return subLocality;
}

String Geocode::getTown()
{
  return town;
}

String Geocode::getCountry()
{
  return country;
}

String Geocode::getFormattedAddress()
{
  return formattedAddress;
}

