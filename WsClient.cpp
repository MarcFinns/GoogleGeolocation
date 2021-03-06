//#include "UserConfig.h"
#include "Wsclient.h"


// Skip HTTP headers so that we are at the beginning of the response's body
bool WsClient::skipResponseHeaders()
{
  bool outcome = false;
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    // Serial.println("HEADERS: " + line);
    if (line == "\r")
    {
      // Serial.println(F("headers received"));
      outcome = true;
      break;
    }
  }
  return outcome;
}


// Open connection to the HTTP server
bool WsClient::httpsConnect(String hostName, String fingerprint)
{
  // Serial.println("Free Heap: " + String(ESP.getFreeHeap()));
  // Serial.println("Connecting to " + hostName);
  bool outcome = client.connect(hostName.c_str(), 443);

  // Serial.println(outcome ? F("Connected") : F("Connection Failed!"));
  if (outcome)
  {
    if (fingerprint != "")
    {
      if (client.verify(fingerprint.c_str(), hostName.c_str()))
      {
        // Serial.println(F("certificate matches"));
      }
      else
      {
        // Serial.println(F("certificate doesn't match"));
        outcome =  false;
      }
    }
  }
  return outcome;
}


// Send the HTTP GET request to the server
bool WsClient::httpsGet(String host, String url)
{
  client.print(F("GET "));
  client.print(url);
  client.println(F(" HTTP/1.1"));
  client.print(F("Host: "));
  client.println(host);
  client.println(F("User-Agent: ESP8266"));
  client.println(F("Content-Type: application/json; charset=UTF-8"));
  client.println(F("Connection: close"));
  client.println();

  return true;
}


// Send the HTTP POST request to the server
bool WsClient::httpsPost(String host, String url, String postData)
{
  client.print(F("POST "));
  client.print(url);
  client.println(F(" HTTP/1.1"));
  client.print(F("Host: "));
  client.println(host);
  client.println(F("Connection: close"));
  client.println(F("Content-Type: application/json"));
  client.println(F("User-Agent: ESP8266"));
  client.print(F("Content-Length: "));
  client.println(postData.length());
  client.println();
  client.print(postData);
}


// Close the connection with the HTTP server
void WsClient::disconnect()
{
  // Serial.println(F("Disconnect"));
  client.stop();
}

