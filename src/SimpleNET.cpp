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
  // Enable bus for TC4
  PM->APBCMASK.reg |= PM_APBCMASK_TC4;

  // Attach GCLK0 to TC4/TC5
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_TC4_TC5 |
                      GCLK_CLKCTRL_GEN_GCLK0 |
                      GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.bit.SYNCBUSY);

  // Software reset TC4
  TC4->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);
  while (TC4->COUNT16.CTRLA.bit.SWRST);

  // MODE=16‑bit, PRESCALER=DIV64, WAVEGEN=MFRQ
  TC4->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16
                         | TC_CTRLA_PRESCALER_DIV64
                         | TC_CTRLA_WAVEGEN_MFRQ;
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);

  // 48 MHz / 64 = 750 kHz → 750 ticks per ms → CC[0] = 749 for 1 ms interrupts
  TC4->COUNT16.CC[0].reg = 749;
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);

  // Enable match interrupt on CC[0]
  TC4->COUNT16.INTENSET.reg = TC_INTENSET_MC0;
  NVIC_EnableIRQ(TC4_IRQn);

  // Start timer
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

void uint64ToString(uint64_t value, char* buf, size_t buflen) {
    if (buflen == 0) return;
    buf[buflen-1] = '\0';
    char tmp[32];
    int i = 30;
    tmp[31] = '\0';
    if (value == 0) {
        strncpy(buf, "0", buflen-1);
        buf[buflen-1] = '\0';
        return;
    }
    while (value > 0 && i >= 0) {
        tmp[i--] = '0' + (value % 10);
        value /= 10;
    }
    strncpy(buf, &tmp[i+1], buflen-1);
    buf[buflen-1] = '\0';
}


uint64_t bytesToUint64_StringDigits(const std::vector<uint8_t>& bytes) {
  uint64_t result = 0;
  for (uint8_t b : bytes) {
    if (b >= '0' && b <= '9') {
      result = result * 10 + (b - '0');
    } else if (b == '\n' || b == '\r' || b == ' ') {
      continue; // ignore whitespace
    } else {
      break; // stop at any non-numeric byte
    }
  }
  return result;
}


String getMac() {
  uint8_t mac[6];
  char macStr[18];
  WiFi.macAddress(mac);
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
}

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
  char timeStr[32];
  uint64ToString(currentTime, timeStr, sizeof(timeStr));

  #ifdef DEBUGTIMESTAMP
  Serial.print("Timestamp: ");
  Serial.println(timeStr);
  #endif

  char absJsonContent[128];
  snprintf(absJsonContent, sizeof(absJsonContent),
      "{\"t_section\":%lu,\"pos_lin\":%lu,\"time\":%s}",
      (unsigned long)data.track_section,
      (unsigned long)data.pos_lin,
      timeStr);


  #ifdef LIGHTWEIGHT
  char message[128];
  snprintf(message, sizeof(message),
      "Car/%s;"
      "pos=%s;",
      getMac().c_str(),
      absJsonContent);
  #else

  char message[256];
  snprintf(message, sizeof(message),
      "Car/%s;"
      "t_section=%lu;"
      "pos/l=%lu;"
      "speed/x=%ld;"
      "speed/y=%ld;"
      "speed/z=%ld;"
      "speed/l=%lu;"
      "accel/x=%ld;"
      "accel/y=%ld;"
      "accel/z=%ld;"
      "accel/l=%ld;"
      "gyros/x=%ld;"
      "gyros/y=%ld;"
      "gyros/z=%ld;"
      "pos=%s;"
      "time=%lu",
      getMac().c_str(),
      (unsigned long)data.track_section,
      (unsigned long)data.pos_lin,
      (unsigned long)data.speed_vec[0],
      (unsigned long)data.speed_vec[1],
      (unsigned long)data.speed_vec[2],
      (unsigned long)data.speed_lin,
      (long)data.accel_vec[0],
      (long)data.accel_vec[1],
      (long)data.accel_vec[2],
      (long)data.accel_lin,
      (long)data.gyro_vec[0],
      (long)data.gyro_vec[1],
      (long)data.gyro_vec[2],
      absJsonContent,
      (unsigned long)currentTime);
  #endif

  udp.beginPacket(SERVER_IP, UDP_PORT);
  udp.write(message);
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


