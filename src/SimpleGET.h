#ifndef SIMPLEGET_H
#define SIMPLEGET_H

#include "SimpleNET.h"

#define HTTP_PORT 8000

std::vector<uint8_t> simpleGET(String path); //Current implementation uses the HTTP GET to retrieve data from the server (at the moment serverside only systemtime possible)

#endif

