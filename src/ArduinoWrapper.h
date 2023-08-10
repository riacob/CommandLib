/**
 * @file ArduinoWrapper.cpp
 * @author your name (you@domain.com)
 * @brief Arduino-specific methods overridden from PlatformWrapper.h
 * @version 0.1
 * @date 2023-08-09
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef ARDUINOWRAPPER_H
#define ARDUINOWRAPPER_H

#include <Arduino.h>
#include "PlatformWrapper.h"

class ArduinoWrapper : public PlatformWrapper
{
public:
    ArduinoWrapper() : PlatformWrapper()
    {
    }
    void readBytes(char *readBuffer, size_t bytesCount) override
    {
        if (Serial.available() < bytesCount)
        {
            debug("Not enough bytes available");
        }
    }
    void readBytesUntil(char *readBuffer, char terminator) override {}
    void writeBytes(char *writeBuffer, size_t bytesCount) override
    {
    }
    void debug(const char *s) override
    {
        Serial.print(s);
    }
        void debugln(const char *s) override
    {
        Serial.println(s);
    }
    void debug( char c) override{
        Serial.print(c);
    }
    void debugln( char c) override{
        Serial.println(c);
    }
};

#endif