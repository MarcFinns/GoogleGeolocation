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
      // Serial.println("headers received");
      outcome = true;
      break;
    }
  }
  return outcome;
}


// Open connection to the HTTP server
bool WsClient::httpsConnect(String hostName, String fingerprint)
{
  bool outcome = client.connect(hostName.c_str(), 443);

  //Serial.println(outcome ? "Connected" : "Connection Failed!");
  if (outcome)
  {
    if (fingerprint != "")
    {
      if (client.verify(fingerprint.c_str(), hostName.c_str()))
      {
        // Serial.println("certificate matches");
      }
      else
      {
        // Serial.println("certificate doesn't match");
        outcome =  false;
      }
    }
  }
  return outcome;
}


// Send the HTTP GET request to the server
bool WsClient::httpsGet(String host, String url)
{
  client.print("GET ");
  client.print(url);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(host);
  client.println("User-Agent: ESP8266");
  client.println("Content-Type: application/json; charset=UTF-8");
  client.println("Connection: close");
  client.println();

  return true;
}


// Send the HTTP POST request to the server
bool WsClient::httpsPost(String host, String url, String postData)
{
  client.print("POST ");
  client.print(url);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(host);
  client.println("Connection: close");
  client.println("Content-Type: application/json");
  client.println("User-Agent: ESP8266");
  client.print("Content-Length: ");
  client.println(postData.length());
  client.println();
  client.print(postData);
}


// Close the connection with the HTTP server
void WsClient::disconnect()
{
  // Serial.println("Disconnect");
  client.stop();
}

