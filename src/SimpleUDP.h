#ifndef SIMPLE_UDP_H
#define SIMPLE_UDP_H

#ifdef ESP32
#include <WiFi.h>
#include <WiFiUdp.h>
#else
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#endif


//Debugging definitions
#define DEBUG 1

//other definitions
#define LABOR 1
#define UDP_PORT 4210


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
#define SERVER_IP "192.168.9.101"
#endif


void SUDP_beginn();
void SUDP_send(String message);
#endif