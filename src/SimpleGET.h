#ifndef SIMPLEGET_H
#define SIMPLEGET_H

#include "SimpleNET.h"

#define HTTP_PORT 8000

std::vector<uint8_t> simpleGET(String path); //get the csv file from the server or the system time /t for systemtime and /filename.csv for the csv file

#endif

