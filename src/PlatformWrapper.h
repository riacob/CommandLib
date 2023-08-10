/**
 * @file PlatformWrapper.h
 * @author your name (you@domain.com)
 * @brief Virtual methods to be defined differently for each platform, i.e Arduino vs STM32.
 * @version 0.1
 * @date 2023-08-09
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef PLATFORMWRAPPER_H
#define PLATFORMWRAPPER_H

class PlatformWrapper
{
public:
    PlatformWrapper() {}
    virtual void readBytes(char *readBuffer, size_t bytesCount) {}
    virtual size_t readBytesUntil(char *readBuffer, char terminator) {}
    virtual void writeBytes(char *writeBuffer, size_t bytesCount, const char *terminators = "") {}
    virtual void debug(const char *s) {}
    virtual void debug( char c) {}
    virtual void debugln(const char *s) {}
    virtual void debugln( char c) {}
};

#endif