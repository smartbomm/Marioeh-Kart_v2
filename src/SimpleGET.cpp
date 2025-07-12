#include "SimpleGET.h"
#include "SimpleNET.h"

WiFiClient client;

std::vector<uint8_t> simpleGET(String path) {
    WIFIstart();
    std::vector<uint8_t> responseBytes;

    if (client.connect(SERVER_IP, HTTP_PORT)) {
        client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                     "Host: " + SERVER_IP + "\r\n" +
                     "Connection: close\r\n\r\n");
    } else {
        return {}; 
    }

    // Skip headers
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r" || line.length() == 1) break;
    }


    while (client.connected() && client.available()) {
        int byteRead = client.read();
        if (byteRead >= 0) {
            responseBytes.push_back(static_cast<uint8_t>(byteRead));
        }
    }

    client.stop();
    return responseBytes;
}
