#include "SimpleGET.h"
#include "SimpleNET.h"


WiFiClient client;

std::vector<String> csvLines;

String simpleGET(String path) {
    WIFIstart();
    csvLines.clear();

    if (client.connect(SERVER_IP, HTTP_PORT)) {
        client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                     "Host: " + SERVER_IP + "\r\n" +
                     "Connection: close\r\n\r\n");
    } else {
        return "-1";  // Return empty -1 on failure
    }

    //header end
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") break;
    }


    String fullCsv;
    while (client.available()) {
        String line = client.readStringUntil('\n');
        line.trim(); 
        if (line.length() > 0) {
            csvLines.push_back(line); 
            fullCsv += line + "\n";
            #ifdef DEBUGCSV
            Serial.println(line);
            #endif
        }
    }

    client.stop();
    #ifdef DEBUGCSV
    Serial.println("ende of transaktione");
    #endif

    return fullCsv;
}
