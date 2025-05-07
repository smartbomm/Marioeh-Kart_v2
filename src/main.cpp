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
arrayFlashStorage(map, coordinate_t,2048);

void setup() 
{
    coordinate_t c [2048];
    map.write(c[0]);
    struct coordinate_t c [remote.data_amount];
    coordinates = c;

    Serial.begin(9600);
    delay(2000);
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