#ifndef ESP32_FSorOTA_h
#define ESP32_FSorOTA_h

#include "Arduino.h"
#include "WiFi.h"
#include "Update.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "FS.h"
#include "LittleFS.h"

class ESP32_FSorOTAClass{
    public:
        void begin(AsyncWebServer *server);

    private:
        AsyncWebServer *_server;
};

extern ESP32_FSorOTAClass ESP32_FSorOTA;

String listFiles();

String humanReadableSize(const size_t bytes);

String cleanLittleFS();

#endif
