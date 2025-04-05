#include "SimpleNET.h"
#include <stdint.h>


#ifdef DEBUGTIME
uint32_t debugCount = 0;
#endif
uint64_t systemTime;


void WIFIstart() {
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(SSID, PASSWORD);
    delay(500u);
  }
}
String uint64ToString(uint64_t value) {
  if (value == 0) return "0";

  char buffer[32];
  int i = 30;
  buffer[31] = '\0';

  while (value > 0 && i >= 0) {
    buffer[i--] = '0' + (value % 10);
    value /= 10;
  }

  return String(&buffer[i + 1]);
}

uint64_t stringToUint64(String input) {
  uint64_t result = 0;
  for (size_t i = 0; i < input.length(); i++) {
    char c = input.charAt(i);
    if (c >= '0' && c <= '9') {
      result = result * 10 + (c - '0');
    } else {
      break;
    }
  }
  return result;
}



#ifdef ENABLE_MQTT
// MQTT Implementation
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    #ifdef DEBUGWIFI
    Serial.print("Attempting MQTT connection...");
    #endif
    
    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      #ifdef DEBUGWIFI
      Serial.println("connected");
      #endif
    } else {
      #ifdef DEBUGWIFI
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      #endif
      delay(5000);
    }
  }
}

void SUDP_beginn(uint64_t u64_time) {
  uint64_t systemTime= micros() + u64_time*1000; // Adjust for system time --> system time is calculated start time of the uC (pulled from the server) + current time of the uC (pulled from the uC)
  #ifdef SERIAL_ENABLE
    Serial.begin(115200);
  #endif
  
  WiFi.begin(SSID, PASSWORD);

  #ifdef DEBUGWIFI
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  #endif

  mqttClient.setServer(SERVER_IP, MQTT_PORT);
}

void SUDP_send(odometerData_t data) {
  #ifdef DEBUGTIME
  debugCount = micros();
  #endif

WIFIstart();

  // Reconnect MQTT if needed
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  // Build the base topic
  String baseTopic = String((WiFi.localIP())) + "/";

  // Publish each value to its own topic
  mqttClient.publish((baseTopic + "t_section").c_str(), String(data.track_section).c_str());
  mqttClient.publish((baseTopic + "pos/x").c_str(), String(data.pos_vec[0]).c_str());
  mqttClient.publish((baseTopic + "pos/y").c_str(), String(data.pos_vec[1]).c_str());
  mqttClient.publish((baseTopic + "pos/z").c_str(), String(data.pos_vec[2]).c_str());
  mqttClient.publish((baseTopic + "pos/l").c_str(), String(data.pos_lin).c_str());
  mqttClient.publish((baseTopic + "speed/x").c_str(), String(data.speed_vec[0]).c_str());
  mqttClient.publish((baseTopic + "speed/y").c_str(), String(data.speed_vec[1]).c_str());
  mqttClient.publish((baseTopic + "speed/z").c_str(), String(data.speed_vec[2]).c_str());
  mqttClient.publish((baseTopic + "speed/l").c_str(), String(data.speed_lin).c_str());
  mqttClient.publish((baseTopic + "accell/x").c_str(), String(data.accel_vec[0]).c_str());
  mqttClient.publish((baseTopic + "accell/y").c_str(), String(data.accel_vec[1]).c_str());
  mqttClient.publish((baseTopic + "accell/z").c_str(), String(data.accel_vec[2]).c_str());
  mqttClient.publish((baseTopic + "accell/l").c_str(), String(data.accel_lin).c_str());
  mqttClient.publish((baseTopic + "time").c_str(), String((micros()+systemTime)).c_str());//TODO: implement time system

  #ifdef DEBUGTIME
  debugCount = micros() - debugCount;
  Serial.print("MQTT publish time: ");
  Serial.print(debugCount);
  Serial.print("\n");
  #endif
}

#else
// UDP Implementation
WiFiUDP udp;
void SUDP_beginn(uint64_t u64_time) {
  systemTime=  u64_time - millis(); // Adjust for system time --> system time is calculated start time of the uC (pulled from the server) - current time of the uC (pulled from the uC)
  #ifdef SERIAL_ENABLE
    Serial.begin(115200);
  #endif
  
  #ifdef DEBUGWIFI
  Serial.println(systemTime);
  #endif

  WIFIstart();


  #ifdef DEBUGWIFI
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  #endif

  udp.begin(UDP_PORT);

  #ifdef DEBUGWiFI
  Serial.print("UDP started on port: ");
  Serial.println(UDP_PORT);
  #endif
}

void SUDP_send(odometerData_t data) {
  #ifdef DEBUGTIME
  debugCount = micros();
  #endif

WIFIstart();

uint64_t currentTime = millis() + systemTime;
String String_currentTime = uint64ToString(currentTime);
  String message = "Car"+String((int)(WiFi.localIP())) + "," 
    + "t_section=" + data.track_section + ","
    + "pos/x=" + data.pos_vec[0] + ","
    + "pos/y=" + data.pos_vec[1] + ","
    + "pos/z=" + data.pos_vec[2] + ","
    + "pos/l=" + data.pos_lin + ","
    + "speed/x=" + data.speed_vec[0] + ","
    + "speed/y=" + data.speed_vec[1] + ","
    + "speed/z=" + data.speed_vec[2] + ","
    + "speed/l=" + data.speed_lin + ","
    + "accel/x=" + data.accel_vec[0] + ","
    + "accel/y=" + data.accel_vec[1] + ","
    + "accel/z=" + data.accel_vec[2] + ","
    + "accel/l=" + data.accel_lin + ","
    + "time=" + String_currentTime; // Adjust for system time

  #ifdef DEBUGTIME
  debugCount = micros();
  #endif
  
  udp.beginPacket(SERVER_IP, UDP_PORT);
  udp.print(message);
  udp.endPacket();

  #ifdef DEBUGTIME
  debugCount = micros() - debugCount;
  Serial.print("UDP transmission time: ");
  Serial.print(debugCount);
  Serial.print("\n");
  #endif
  
  #ifdef DEBUGSENT
  Serial.println(message);
  #endif
}
#endif



