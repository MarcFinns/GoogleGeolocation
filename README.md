# GoogleGeolocation
Uses Google maps API and NTP to determine time, timezone, daylight saving, latitude/longitude, physical address (still work in progress, code not beautiful but works)
The 3 Google APIs utilised (Geolocate, Timezone, Geocode) are wrapped in separate classes so that they can be used separately if needed.
Uses memory efficient parser as Google APIs return huge JSON. 
Please see example on how to instantiate and delete objects, to avoid overflowing the heap...
