#ifndef SIMPLE_NET_H
#define SIMPLE_NET_H


#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <stdint.h>
#include <Arduino.h>
#include <vector>


#include "OdometerData.h"

//Debugging definitions
//#define DEBUG 1
//#define DEBUGWIFI 1
//#define DEBUGCSV 1
//#define DEBUGSENT 1
 //#define SERIAL_ENABLE 1
//#define DEBUGTIME 1
//#define DEBUGTIMESTAMP 1

//#define LIGHTWEIGHT 1




//other definitions
#define LABOR 1 



#define UDP_PORT 4210 //port for the udp com to the MQTT bridge



#ifdef LABOR // if you use other wifi settings, you can add them here
#define SSID "Labore-Hof AT-MT"
#define PASSWORD "laborwlan"
#define SERVER_IP "192.168.9.150"
#endif



void SUDP_beginn(uint64_t u64_time);//give the time in microseconds to the function so it can sync the time to the system
void SUDP_send(odometerData_t data);
void WIFIstart(); //start the wifi connection (with configurations defined above)
uint64_t bytesToUint64_StringDigits(const std::vector<uint8_t>& bytes); //convert a string to a uint64_t
uint32_t accurateMillis(); //get the accurate millis from the timer
#endif