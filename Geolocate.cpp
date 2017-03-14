
#include "ESP8266WiFi.h"
#include <NtpClientLib.h>         // https://github.com/gmag11/NtpClient
#include <JsonStreamingParser.h>
#include "UserConfig.h"
#include "GoogleGeolocation.h"


#define min(a,b) ((a)<(b)?(a):(b))


/**********************************************************
   Step 1 - Geolocation
   Get latitude and longitude via Wifi triangulation
 **********************************************************/
bool Geolocate::acquire()
{

  // Get precise location via WiFi triangulation
  // Serial.println(F("WiFI scan start"));

  char bssid[6];

  // SCAN AVAILABLE NETWORKS
  int n = min(WiFi.scanNetworks(false, true), MAX_SSIDS);

  // Serial.println(F("scan done"));

  // Found any?
  if (n == 0)
  {
    // Serial.println(F("no networks found, resorting to IP address only"));
  }
  else
  {
    // Serial.print(n);
    // Serial.println(F(" networks found..."));

    // Build the postData for Google Geolocation API...
    String postData = F("{\n \"considerIp\": \"true\", \n \"wifiAccessPoints\": [\n");
    for (int j = 0; j < n; ++j)
    {
      postData += F("{\n");
      postData += F("\"macAddress\" : \"");
      postData += (WiFi.BSSIDstr(j));
      postData += F("\",\n");
      postData += F("\"signalStrength\": ");
      postData += WiFi.RSSI(j);
      postData += F("\n");
      if (j < n - 1)
      {
        postData += F("},\n");
      }
      else
      {
        postData += F("}\n");
      }
    }
    postData += F("]\n");
    postData += F("}\n");

    // Serial.println(F("Connecting..."));
    //Connect to the client and make the api call
    if (httpsConnect(FPSTR(geolocation_Host), ""))
    {
      // Serial.println(F("Connected..."));

      String url = FPSTR(geolocation_url);
      url += FPSTR(googleApiKey);

      if (httpsPost(FPSTR(geolocation_Host), url, postData) && skipResponseHeaders())
      {
        // Serial.println(F("Posted..."));
        JsonStreamingParser parser;
        parser.setListener(this);
        char c;
        int size = 0;

        while ((size = client.available()) > 0)
        {
          c = client.read();
          parser.parse(c);
        }
        // Serial.println(F("Parsed..."));
      }
      else
      {
        // Post failed
        return false;
      }
      disconnect();
    }
    else
      // Could not connect
      return false;
  }

  valid = true;

  return true;
}

bool Geolocate::isValid()
{
  return valid;
}

void Geolocate::whitespace(char c) {
  // Serial.println(F("whitespace"));
}

void Geolocate::startDocument() {
  // Serial.println(F("start document"));
}

void Geolocate::key(String key)
{
  // Serial.println("key: " + key);
  currentKey = String(key);

}

void Geolocate::value(String value)
{
  // Serial.println("value: " + value);
  if (currentParent == F("location"))
  {
    if (currentKey == F("lat"))
    {
      latitude = value.toFloat();
    }
    else if (currentKey == F("lng"))
    {
      longitude = value.toFloat();
    }
  }
}

void Geolocate::endArray() {
  // Serial.println(F("end array. "));
}

void Geolocate::endObject()
{
  // Serial.println(F("end object. "));
  currentParent = "";
}

void Geolocate::endDocument() {
  // Serial.println(F("end document. "));
}

void Geolocate::startArray() {
  // Serial.println(F("start array. "));
}

void Geolocate::startObject()
{
  // Serial.println(F("start object. "));
  currentParent = currentKey;
}

double Geolocate::getLatitude()
{
  return latitude;
}

double Geolocate::getLongitude()
{
  return longitude;
}




