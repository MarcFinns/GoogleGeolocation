
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
  //Serial.println("WiFI scan start");

  char bssid[6];

  // SCAN AVAILABLE NETWORKS
  int n = min(WiFi.scanNetworks(false, true), MAX_SSIDS);

  //Serial.println("scan done");

  // Found any?
  if (n == 0)
  {
    // Serial.println("no networks found, resorting to IP address only");
  }
  else
  {
    //Serial.print(n);
    //Serial.println(" networks found...");

    // Build the postData for Google Geolocation API...
    String postData = "{\n \"considerIp\": \"true\", \n \"wifiAccessPoints\": [\n";
    for (int j = 0; j < n; ++j)
    {
      postData += "{\n";
      postData += "\"macAddress\" : \"";
      postData += (WiFi.BSSIDstr(j));
      postData += "\",\n";
      postData += "\"signalStrength\": ";
      postData += WiFi.RSSI(j);
      postData += "\n";
      if (j < n - 1)
      {
        postData += "},\n";
      }
      else
      {
        postData += "}\n";
      }
    }
    postData += ("]\n");
    postData += ("}\n");

    //Connect to the client and make the api call
    if (httpsConnect(geolocation_Host, ""))
    {
      if (httpsPost(geolocation_Host, geolocation_url + googleApiKey, postData) && skipResponseHeaders())
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
  //Serial.println("whitespace");
}

void Geolocate::startDocument() {
  // Serial.println("start document");
}

void Geolocate::key(String key)
{
  // Serial.println("key: " + key);
  currentKey = String(key);

}

void Geolocate::value(String value)
{
  // Serial.println("value: " + value);
  if (currentParent == "location")
  {
    if (currentKey == "lat")
    {
      latitude = value.toFloat();
    }
    else if (currentKey == "lng")
    {
      longitude = value.toFloat();
    }
  }
}

void Geolocate::endArray() {
  // Serial.println("end array. ");
}

void Geolocate::endObject()
{
  // Serial.println("end object. ");
  currentParent = "";
}

void Geolocate::endDocument() {
  // Serial.println("end document. ");
}

void Geolocate::startArray() {
  // Serial.println("start array. ");
}

void Geolocate::startObject()
{
  // Serial.println("start object. ");
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




