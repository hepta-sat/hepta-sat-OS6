#include "SerialBuffered.h"

/**
     * Create a buffered serial class.
     *
     * @param tx A pin for transmit.
     * @param rx A pin for receive.
     * @param baud Baud rate (default: 9600)
     */
SerialBuffered::SerialBuffered(PinName tx, PinName rx, int baud)
    : BufferedSerial(tx, rx, baud) {
    indexContentStart = 0;
    indexContentEnd = 0;
    timeout = 1;

    set_baud(baud);
    set_blocking(false);
}

SerialBuffered::~SerialBuffered() {
}

/** Set timeout for getc() (ms, -1:disable) */
void SerialBuffered::setTimeout(int ms) {
    timeout = ms;
}

/** Read requested bytes */
size_t SerialBuffered::readBytes(uint8_t *bytes, size_t requested) {
    size_t i = 0;
    timer.reset();
    timer.start();

    while (i < requested) {
        char c;
        ssize_t n = this->read(&c, 1);

        if (n == 1) {
            bytes[i++] = static_cast<uint8_t>(c);
        } else {
            // Data not available yet, wait briefly
            ThisThread::sleep_for(1ms);
        }

        if (timeout > 0 &&
            timer.elapsed_time() > std::chrono::milliseconds(timeout)) {
            break;  // Timeout
        }
    }

    timer.stop();
    return i;
}

/** Get a character. (-1:timeout) */
int SerialBuffered::getc() {
    timer.reset();
    timer.start();

    while (true) {
        char c;
        ssize_t n = this->read(&c, 1);
        if (n == 1) {
            return static_cast<uint8_t>(c);
        }

        ThisThread::sleep_for(1ms);

        if (timeout > 0 &&
            timer.elapsed_time() > std::chrono::milliseconds(timeout)) {
            timer.stop();
            return EOF;  // Timeout
        }
    }
}

/** Returns 1 if there is a character available to read, 0 otherwise. */
int SerialBuffered::readable() {
    return BufferedSerial::readable() ? 1 : 0;
}

void SerialBuffered::handleInterrupt() {
    // Not used in the OS6 version
}

void SerialBuffered::_baud(int b) {
    set_baud(b);
}
