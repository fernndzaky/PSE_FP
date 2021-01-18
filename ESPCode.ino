 /*
    HTTP over TLS (HTTPS) example sketch

    This example demonstrates how to use
    WiFiClientSecure class to access HTTPS API.
    We fetch and display the status of
    
    esp8266/Arduino project continuous integration
    build.

    Limitations:
      only RSA certificates
      no support of Perfect Forward Secrecy (PFS)
      TLSv1.2 is supported since version 2.4.0-rc1

    Created by Ivan Grokhotkov, 2015.
    This example is in public domain.
*/

#include <ESP8266WiFi.h>
//#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 1);  // RX, TX

#ifndef STASSID
#define STASSID "M.E.S"
#define STAPSK  "password39"
#endif



const char* ssid = STASSID;
const char* password = STAPSK;

//const char* host = "api.github.com";
//const int httpsPort = 443;


// Use web browser to view and copy
// SHA1 fingerprint of the certificate
//const char fingerprint[] PROGMEM = "5F F1 60 31 09 04 3E F2 90 D2 B0 8A 50 38 04 E8 37 9F BC 76";

String data = "";

void setup() {
  pinMode(1, OUTPUT);
  //vmySerial.begin(115200);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED)
    connectWifi();
}

void connectWifi() {
   Serial.println();
   Serial.print("connecting to ");
   Serial.println(ssid);
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
       digitalWrite(1, HIGH);
       delay(500);
       digitalWrite(1, LOW);
       Serial.print(".");
   }
   Serial.println("");
   Serial.println("WiFi connected");
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
}

void loop() {
    // Connect to wifi if disconnected
    while (WiFi.status() != WL_CONNECTED) connectWifi();

    // Read string from serial
    bool isDelimiterFound = false;
    
//    while (Serial.available() > 0 ) {
//       char c = Serial.read();
//       if (c == ';') {
//        isDelimiterFound = true;
//        break;
//       }
//       else {
//        data += c;        
//       }
//    }
      while (mySerial.available()) {
        String incomingString = mySerial.readString();
        for (int i = 0; i < incomingString.length(); i++) {
           char c = incomingString[i];
           if (c == ';') {
              isDelimiterFound = true;
              break;
           }
           else {
              data += c;        
           }
        }
      }

    if (isDelimiterFound) {
        // Send data to server
        String serverUrl = "http://chickennamban.000webhostapp.com/Homies_Garage/database/add_item.php?item_id=";
        Serial.println(serverUrl + data);
        HTTPClient http;
        http.begin(serverUrl + data);
        http.addHeader("Content-Type", "text/plain");
        int httpResponseCode = http.POST("POSTING from ESP32");
    
        // Check response code from server
        if(httpResponseCode > 0) {
              String response = http.getString();
            
              Serial.println(httpResponseCode);
              Serial.println(response);
            
        } else {
              Serial.print("Error on sending POST: ");
              Serial.println(httpResponseCode);
        }
        
        http.end();  //Free resources
        data = "";
        delay(100);
    }
}
