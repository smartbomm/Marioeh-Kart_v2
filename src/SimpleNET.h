#ifndef SIMPLE_NET_H
#define SIMPLE_NET_H


#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <stdint.h>
#include <sam.h>


#include "OdometerData.h"

//Debugging definitions
//#define DEBUG 1
//#define DEBUGWIFI 1
//#define DEBUGCSV 1
//#define DEBUGSENT 1
//#define SERIAL_ENABLE 1
//#define DEBUGTIME 1
//#define DEBUGTIMESTAMP 1
//#define ENABLE_MQTT

/*
#define DEBUGTIMETOSEND Serial.print(x)
#define DEBUGSENTMESSAGE Serial.print(x)
#define DEBUGWIFI Serial.print(x)
*/


//other definitions
#define LABOR 1


//#define ENABLE_MQTT 1

#ifdef ENABLE_MQTT
  // MQTT Configuration
  #include <PubSubClient.h>
  #define MQTT_PORT 1883
  #define MQTT_CLIENT_ID "CarClient"
  #define MQTT_USER "MARIOE"
  #define MQTT_PASSWORD "CART"
#else
#define UDP_PORT 4210
#endif



#ifdef DAHEIM
#define SSID "WLAN"
#define PASSWORD "06130705342415756002"
#define SERVER_IP "192.168.178.42"
#endif

#ifdef HOFSPANNUNG
#define SSID "HofSpannung"
#define PASSWORD "8ZfjM4GHYVgrYa"
#define SERVER_IP "192.168.1.62"
#endif

#ifdef LABOR
#define SSID "Labore-Hof AT-MT"
#define PASSWORD "laborwlan"
#define SERVER_IP "192.168.9.150"
#endif

#ifdef HOF
#define SSID "karmvc"
#define PASSWORD "jvTtFfN3wC8g4UcQ"
#define SERVER_IP "192.168.178.112"
#endif


void SUDP_beginn(uint64_t u64_time);//gibe the time in microseconds to the function so it can sync the time to the system
uint64_t stringToUint64(String input); //convert a string to a uint64_t
String uint64ToString(uint64_t input); //convert a uint64_t to a string
void SUDP_send(odometerData_t data);
void WIFIstart();
#endif