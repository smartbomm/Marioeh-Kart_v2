#include "SimpleUdp.h"
//#include "OdometerData.h"

#ifdef DEBUG
uint32_t debugCount = 0;
#endif

WiFiUDP udp;

void SUDP_beginn() {
  #ifdef DEBUG
    Serial.begin(115200);
  #endif
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500u);
        #ifdef DEBUG
        Serial.print(".");
        #endif
    }

    #ifdef DEBUG
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    #endif

    udp.begin(UDP_PORT);

    #ifdef DEBUG
    Serial.print("UDP started on port: ");
    Serial.println(UDP_PORT);
    #endif
}

void SUDP_send(String message) {

  while (WiFi.status() != WL_CONNECTED) {
    delay(500u);
    #ifdef DEBUG
    Serial.print("reconnecting...");
    #endif

}
    #ifdef DEBUG
    debugCount = micros();
    #endif

    udp.beginPacket(SERVER_IP, UDP_PORT);
    udp.print(message);
    udp.endPacket();


    #ifdef DEBUGTIME
    debugCount = micros() - debugCount;
    Serial.print("Transmission time: ");
    Serial.print(debugCount);
    Serial.print("\n");

    #endif
    #ifdef DEBUGSENT
    Serial.println(message);
    #endif
    
}
// TODO void Tostring