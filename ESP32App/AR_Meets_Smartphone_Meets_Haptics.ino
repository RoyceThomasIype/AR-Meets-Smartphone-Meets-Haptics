/**
 * AR_Meets_Smartphone_Meets_Haptics.ino
 *
 *  Created on: 12.09.2022
 *
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#define USE_SERIAL Serial

WiFiMulti wifiMulti;
uint8_t hapticSensor = 2;

void setup() {
    USE_SERIAL.begin(115200);
    Serial.println("AR Meets Smarphone Meets Haptics!");
    USE_SERIAL.println();
    pinMode(hapticSensor, OUTPUT);
    
    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
//    wifiMulti.addAP("Pixel 6 Pro", "g7qkhncrspf83bt");
    wifiMulti.addAP("TP-Link_782F", "20250423");
}

void loop() {
    // wait for WiFi connection
    if((wifiMulti.run() == WL_CONNECTED)) {
        HTTPClient http;
//        http.begin("https://smartphone-meets-ar-haptics-default-rtdb.europe-west1.firebasedatabase.app/arHaptics.json"); //HTTP
//        http.begin("http://10.48.128.82/get_hapticVal.php?"); //HTTP
//        http.begin("http://192.168.126.22/get_hapticVal.php?"); //HTTP
        http.begin("http://192.168.0.203/get_hapticVal.php?"); //HTTP
        

        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
//                Serial.println(payload);
                JSONVar myObject = JSON.parse(payload);
                if (myObject.hasOwnProperty("haptics")) {            
//                  Serial.println(myObject["haptics"]);
                  String hapticVal = JSON.stringify(myObject["haptics"]);
                  Serial.println(hapticVal);
                  if(hapticVal.equals("\"on\"")) {
                      digitalWrite(hapticSensor, HIGH);
                  } else {
                      digitalWrite(hapticSensor, LOW);
                  }
                }
                
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
}
