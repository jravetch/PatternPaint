#ifndef BLINKYTAPE_H
#define BLINKYTAPE_H

#include <QObject>
#include <QList>
#include <QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>

/// Some defines that should go into a processor-specific class
#define FLASH_MEMORY_AVAILABLE 0x7000  // Amount of application space in the flash
#define FLASH_MEMORY_PAGE_SIZE 0x80    // Size of a page of memory in our flash

#define BLINKYTAPE_SKETCH_VID 0x1D50
#define BLINKYTAPE_SKETCH_PID 0x605E
#define BLINKYTAPE_BOOTLOADER_VID 0x1D50
#define BLINKYTAPE_BOOTLOADER_PID 0x606C

#define LEONARDO_SKETCH_VID 0x2341
#define LEONARDO_SKETCH_PID 0x8036
#define LEONARDO_BOOTLOADER_VID 0x2341
#define LEONARDO_BOOTLOADER_PID 0x0036

/// Connect to a BlinkyTape over a serial port, and manage sending data to it.
class BlinkyTape : public QObject
{
    Q_OBJECT
public:
    static QList<QSerialPortInfo> findBlinkyTapes();
    static QList<QSerialPortInfo> findBlinkyTapeBootloaders();

    BlinkyTape(QObject *parent = 0, int ledCount = 60);

    // TODO: Destructor!

    bool isConnected();

    bool open(QSerialPortInfo info);
    void close();

    void sendUpdate(QByteArray colors);

    bool getPortInfo(QSerialPortInfo &info);

    // Atempt to reset the strip by setting it's baud rate to 1200 and closing it.
    void reset();

private:
    /// Serial port the BlinkyTape is connected to
    QSerialPort* serial;

    // Number of LEDs on the BlinkyTape
    int ledCount;

#if defined(Q_OS_WIN)
    // Windows only: Timer that periodically checks if the serial device is
    // still present (it seems to disappear without sending an error?)
    // Handle this by periodically scanning for available BlinkyTapes, and
    // close this one if it no longer in the list
    // TODO: Pull the latest QtSerialPort, it seems to have a fix for this
    QTimer *connectionScannerTimer;
#endif

signals:
    void connectionStatusChanged(bool status);

private slots:
    void handleSerialError(QSerialPort::SerialPortError error);

#if defined(Q_OS_WIN)
    void handleConnectionScannerTimer();
#endif
};

#endif // BLINKYTAPE_H
