#ifndef _SERIAL_BUFFERED_H_
#define _SERIAL_BUFFERED_H_

#include "mbed.h"

using namespace mbed;

/**
 * Buffered serial class (mbed OS 6)
 */
class SerialBuffered : public BufferedSerial {
public:
    /**
     * Create a buffered serial class.
     *
     * @param tx A pin for transmit.
     * @param rx A pin for receive.
     * @param baud Baud rate (default: 9600)
     */
    SerialBuffered(PinName tx, PinName rx, int baud = 9600);

    virtual ~SerialBuffered();

    /** Get a character. (-1:timeout) */
    int getc();

    /** Returns 1 if there is a character available to read */
    int readable();

    /** Set timeout for getc() (ms, -1:disable) */
    void setTimeout(int ms);

    /** Read requested bytes */
    size_t readBytes(uint8_t *bytes, size_t requested);

    void _baud(int b);

private:
    void handleInterrupt();
    static const int BUFFERSIZE = 4096;
    uint8_t buffer[BUFFERSIZE];   // points at a circular buffer, containing data from m_contentStart, for m_contentSize bytes, wrapping when you get to the end
    uint16_t indexContentStart;   // index of first bytes of content
    uint16_t indexContentEnd;     // index of bytes after last byte of content
    int timeout;
    Timer timer;
};

#endif
