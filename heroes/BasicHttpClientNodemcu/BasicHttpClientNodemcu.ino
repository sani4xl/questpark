/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

int ledPort = 5;
int btnPort = 4;
int activate = 0;
int lastPingTime = 0;

void setup() {

    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);
    pinMode(ledPort, OUTPUT);
    pinMode(btnPort, INPUT);
    digitalWrite(ledPort, LOW);  
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    lastPingTime = millis();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("Batman", "Haha12345");

}

boolean stateChanged = false;
void loop() {

    
    if(digitalRead(btnPort)){
      activate = activate ? 0 : 1; 
      USE_SERIAL.println("button pressed");
      stateChanged =  true;
      delay(250);
      lastPingTime = 0;
    }

    if(activate){
                    digitalWrite(ledPort, HIGH);
                  }
                  else{
                    digitalWrite(ledPort, LOW);
                  }

    //*
    // wait for WiFi connection
    if( (millis() - lastPingTime) >= 10 * 1000  ){ // 10 sec
      lastPingTime = millis();
    
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        String url = "http://192.168.1.131:8080/module-info?id=module1";
        if(stateChanged){
          url = "http://192.168.1.131:8080/activate_module?id=module1&state=";
          url.concat(activate);
          USE_SERIAL.println(url);
          stateChanged = false;
        }
        http.begin(url); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK && !stateChanged) {
                String payload = http.getString();
                USE_SERIAL.println(payload);

                StaticJsonBuffer<400> jsonBuffer;
                // If the memory pool is too big for the stack, use this instead:
                // DynamicJsonBuffer jsonBuffer;

                JsonObject& root = jsonBuffer.parseObject(payload);
                if (!root.success()) {
                  Serial.println("JSON parsing failed!");
                  //return false;
                }
                else{
                  activate = root["activate"];
                  //Serial.println(activate);
                }
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
        lastPingTime = millis();
    }
    else{
      USE_SERIAL.println("not connected");
    }

    }
    //*/
    

    
}

