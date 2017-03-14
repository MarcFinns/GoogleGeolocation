#pragma once

#define MAX_SSIDS 8

const char PROGMEM  googleApiKey[] = "YOUR API KEY";

const char PROGMEM  geolocation_Host[] = "www.googleapis.com";
const char PROGMEM  geolocation_url[] = "/geolocation/v1/geolocate?key=";

// SHA1 fingerprint of the maps.googleapi.com certificate (optional)
const char PROGMEM  maps_fingerprint[] = "";

const char PROGMEM maps_Host[] = "maps.googleapis.com";

const char PROGMEM timeZone_url[] = "/maps/api/timezone/json?location=";

const char PROGMEM geocoding_url[] = "/maps/api/geocode/json?latlng=";

const char PROGMEM empty[] = "";

