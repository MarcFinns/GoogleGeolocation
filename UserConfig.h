#pragma once

#include <ESP8266WiFi.h>

// Geolocation

#define MAX_SSIDS 10

static const String googleApiKey = "GOOGLE API KEY";

static const String geolocation_Host = "www.googleapis.com";
static const String geolocation_url = "/geolocation/v1/geolocate?key=";

// Time zone and Geocoding

// SHA1 fingerprint of the maps.googleapi.com certificate
static const String maps_fingerprint = "22 37 4D 58 43 F4 A1 24 12 71 2B 74 7A FC 36 FC 24 A0 F0 9D";

static const String maps_Host = "maps.googleapis.com";
static const String  timeZone_url = "/maps/api/timezone/json?location=";

static const String  geocoding_url = "/maps/api/geocode/json?latlng=";

