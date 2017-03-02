#include <NtpClientLib.h>         // https://github.com/gmag11/NtpClient
#include <ESP8266WiFi.h>

#include "UserConfig.h"
#include "Geolocate.h"
#include "Timezone.h"
#include "Geocode.h"

// NTP Server:
const char ntpServerName[] = "pool.ntp.org";
bool ntpAcquired = false;

// Values acquired from Google
double latitude;
double longitude;
int dstOffset;
int rawOffset;
String timeZoneId;
String timeZoneName;
String street;
String streetNumber;
String postCode;
String subLocality;
String town;
String country;
String formattedAddress;

void setup()
{
  Serial.begin(250000);
  Serial.println("START - Free Heap: " + String(ESP.getFreeHeap()));

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(myssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(myssid, mypass);

  // Wait for connection...
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(".");
  Serial.println("Connected to network " + String(WiFi.SSID()) + " with address " + String(WiFi.localIP().toString()));

  // Setup NTP
  NTP.onNTPSyncEvent([](NTPSyncEvent_t event)
  {
    Serial.println();
    Serial.println( "Sync event");
    if (event)
    {
      if (event == noResponse)
        Serial.println( "NTP server not reachable");
      else if (event == invalidAddress)
        Serial.println("Invalid NTP server address");
    }
    else
    {
      ntpAcquired = true;
      Serial.println("Got NTP time - " + NTP.getTimeDateString(NTP.getLastNTPSync()));
    }
  });

  // Start NTP sync
  Serial.println("1 ------- Sincronising time via NTP on UTC time zone...");
  NTP.begin(ntpServerName, 0, false);
  NTP.setInterval(5, 1800);

  // Wait until NTP time is synchronised (UTC, needed to determine timezone)
  while (NTP.getLastNTPSync() == 0)
  {
    Serial.print(".");
    delay(500);
    NTP.getTimeDateString();
  }

  // Google Geolocation -  Acquire coordinates

  Serial.println("2 ------- Retrieving coordinates...");
  Geolocate* geolocate = new Geolocate();
  geolocate->acquire();

  latitude = geolocate->getLatitude();
  longitude = geolocate->getLongitude();

  // Release memory
  delete geolocate;

  // Google Timezone - Acquire timezone and daylight saving
  Serial.println("3 ----------- Retrieving timezone...");

  Timezone* timezone = new Timezone();
  timezone->acquire(latitude, longitude, String(now()));

  // Restart NTP according to local time zone / DST
  Serial.println("4 ------- Sincronising time via NTP on local time zone...");
  //NTP.stop();
  NTP.begin(ntpServerName, timezone->getRawOffset() / 3600, timezone->getDstOffset() != 0);
  NTP.setInterval(5, 1800);

  // Wait until NTP time is synchronised
  Serial.println(NTP.getTimeDateString());
  while (NTP.getLastNTPSync() == 0)
  {
    Serial.print(".");
    delay(500);
    NTP.getTimeDateString();
  }

  // Save variables
  dstOffset = timezone->getRawOffset();
  rawOffset = timezone->getDstOffset();
  timeZoneId = timezone->getTimeZoneId();
  timeZoneName = timezone->getTimeZoneName();

  // Release memory
  delete timezone;


  //3 - Acquire address

  Serial.println("5 ----------- Acquiring geo address...");
  Geocode* geocode = new Geocode();
  geocode->acquire(latitude, longitude);

  // Save variables
  street = geocode->getStreet();
  streetNumber = geocode->getStreetNumber();
  postCode = geocode->getPostCode();
  subLocality = geocode->getSubLocality();
  town = geocode->getTown();
  country = geocode->getCountry();
  formattedAddress = geocode->getFormattedAddress();

  // Release memory
  delete geocode;

  Serial.println("END - Free Heap: " + String(ESP.getFreeHeap()));
}

void loop()
{

  Serial.println( "======== TIME ==================");
  Serial.print(NTP.getTimeDateString()); Serial.print(" ");
  Serial.println(NTP.isSummerTime() ? "Summer Time. " : "Winter Time. ");
  
  Serial.println( "======== TIME ZONE ==================");
  Serial.println( "Raw Offset = " + String(rawOffset));
  Serial.println( "DST offset = " + String(dstOffset));
  Serial.println( "Time Zone ID = " + timeZoneId);
  Serial.println( "Time Zone Name = " + timeZoneName);

  Serial.println( "======== COORDINATES ==================");
  Serial.println( "Latitude = " + String(latitude));
  Serial.println( "Longitude = " + String(longitude));

  Serial.println( "======== ADDRESS ==================");
  Serial.println("street = " + street);
  Serial.println( "streetNumber = " + streetNumber);
  Serial.println( "postCode = " + postCode);
  Serial.println( "subLocality = " + subLocality);
  Serial.println( "town = " + town);
  Serial.println("country = " + country);
  Serial.println( formattedAddress);

  Serial.println( "==========================");
  Serial.println("Loop - Free Heap: " + String(ESP.getFreeHeap()));

  while (1)
  {
    delay(5000);
  }
}


