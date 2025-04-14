#include "SimpleNET.h"

#ifdef DEBUGTIME
uint32_t debugCount = 0;
#endif
uint64_t systemTime;

volatile uint32_t exactMillis = 0;

void TC4_Handler() {
  if (TC4->COUNT16.INTFLAG.bit.MC0) {
    TC4->COUNT16.INTFLAG.reg = TC_INTFLAG_MC0;
    exactMillis++;
  }
}

void startAccurateMillisTimer() {
  PM->APBCMASK.reg |= PM_APBCMASK_TC4;

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_TC4_TC5 |
                      GCLK_CLKCTRL_GEN_GCLK0 |
                      GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.bit.SYNCBUSY);

  TC4->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);
  while (TC4->COUNT16.CTRLA.bit.SWRST);

  TC4->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 |
                           TC_CTRLA_PRESCALER_DIV64 |
                           TC_CTRLA_WAVEGEN_MFRQ;
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);

  TC4->COUNT16.CC[0].reg = 750;
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);

  TC4->COUNT16.INTENSET.reg = TC_INTENSET_MC0;
  NVIC_EnableIRQ(TC4_IRQn);

  TC4->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);
}

uint32_t accurateMillis() {
  noInterrupts();
  uint32_t t = exactMillis;
  interrupts();
  return t;
}

void WIFIstart() {
  while (WiFi.status() != WL_CONNECTED) {
    #ifdef DEBUGWIFI
    Serial.println("Connecting to WiFi...");
    #endif
    WiFi.begin(SSID, PASSWORD);
    delay(1000u);
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

String getMac(){
  uint8_t mac[6];
  WiFi.macAddress(mac);
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
}

#ifdef ENABLE_MQTT
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
  startAccurateMillisTimer();
  systemTime = u64_time - accurateMillis();

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

  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  String baseTopic = getMac() + "/";
  uint64_t currentTime = accurateMillis() + systemTime;
  String String_currentTime = uint64ToString(currentTime);

  #ifdef DEBUGTIMESTAMP
  Serial.print("Timestamp: ");
  Serial.println(String_currentTime);
  #endif

  mqttClient.publish((baseTopic + "t_section").c_str(), String(data.track_section).c_str());
  mqttClient.publish((baseTopic + "pos/x").c_str(), String(data.pos_vec[0]).c_str());
  mqttClient.publish((baseTopic + "pos/y").c_str(), String(data.pos_vec[1]).c_str());
  mqttClient.publish((baseTopic + "pos/z").c_str(), String(data.pos_vec[2]).c_str());
  mqttClient.publish((baseTopic + "pos/l").c_str(), String(data.pos_lin).c_str());
  mqttClient.publish((baseTopic + "speed/x").c_str(), String(data.speed_vec[0]).c_str());
  mqttClient.publish((baseTopic + "speed/y").c_str(), String(data.speed_vec[1]).c_str());
  mqttClient.publish((baseTopic + "speed/z").c_str(), String(data.speed_vec[2]).c_str());
  mqttClient.publish((baseTopic + "speed/l").c_str(), String(data.speed_lin).c_str());
  mqttClient.publish((baseTopic + "accel/x").c_str(), String(data.accel_vec[0]).c_str());
  mqttClient.publish((baseTopic + "accel/y").c_str(), String(data.accel_vec[1]).c_str());
  mqttClient.publish((baseTopic + "accel/z").c_str(), String(data.accel_vec[2]).c_str());
  mqttClient.publish((baseTopic + "accel/l").c_str(), String(data.accel_lin).c_str());
  mqttClient.publish((baseTopic + "time").c_str(), String_currentTime.c_str());

  #ifdef DEBUGTIME
  debugCount = micros() - debugCount;
  Serial.print("MQTT publish time: ");
  Serial.println(debugCount);
  #endif
}

#else
WiFiUDP udp;

void SUDP_beginn(uint64_t u64_time) {
  startAccurateMillisTimer();
  systemTime = u64_time - accurateMillis();

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

  uint64_t currentTime = accurateMillis() + systemTime;
  String String_currentTime = uint64ToString(currentTime);

  #ifdef DEBUGTIMESTAMP
  Serial.print("Timestamp: ");
  Serial.println(String_currentTime);
  #endif

  String message = getMac() + ","
    + "t_section=" + data.track_section + ","
    + "pos/x=" + data.pos_vec[0] + ","
    + "pos/y=" + data.pos_vec[1] + ","
    + "pos/z=" + data.pos_vec[2] + ","
    + "pos/l=" + data.pos_lin + ","
    + "gyros/x=" + data.speed_vec[0] + ","
    + "gyros/y=" + data.speed_vec[1] + ","
    + "gyros/z=" + data.speed_vec[2] + ","
    + "speed/l=" + data.speed_lin + ","
    + "accel/x=" + data.accel_vec[0] + ","
    + "accel/y=" + data.accel_vec[1] + ","
    + "accel/z=" + data.accel_vec[2] + ","
    + "accel/l=" + data.accel_lin + ","
    + "time=" + String_currentTime;

  udp.beginPacket(SERVER_IP, UDP_PORT);
  udp.print(message);
  udp.endPacket();

  #ifdef DEBUGTIME
  debugCount = micros() - debugCount;
  Serial.print("UDP transmission time: ");
  Serial.println(debugCount);
  #endif

  #ifdef DEBUGSENT
  Serial.println(message);
  #endif
}
#endif
