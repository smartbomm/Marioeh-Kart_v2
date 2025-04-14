#include <Arduino.h>
#include <FlashStorage_SAMD.h>
#include <FlashStorage.h>

#define Version 0x34

struct map_config_t {
    uint16_t version;
    uint32_t data_amount;
} remote, local;

struct coordinate_t {
    uint16_t x;
    uint16_t y;
};

coordinate_t *coordinates;

FlashStorage(test_sto, map_config_t);

void setup() 
{
    struct coordinate_t c [remote.data_amount];
    coordinates = c;

    Serial.begin(9600);
    delay(2000);
    remote.token = 0x4B7D9F71u;
    remote.version = 0x34;
    test_sto.read(local);
    if(local.version == remote.version)
    {
        Serial.println("Bereits gespeichert");
        for(;;);
    }
    else
    {
        test_sto.write(remote);
        Serial.println("Data written");
        for(;;);
    }
}

void loop()
{

}