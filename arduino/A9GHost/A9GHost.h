#ifndef A9G_HOST_H
#define A9G_HOST_H

#include <Arduino.h>
#include <Stream.h>

class A9GHost {
public:
  explicit A9GHost(Stream &serial);

  void setDebug(Stream *debug);
  void flushInput();

  bool ping(uint32_t timeoutMs = 1000);
  bool disableEcho(uint32_t timeoutMs = 1000);
  bool getIMEI(String &imei, uint32_t timeoutMs = 1000);
  bool getICCID(String &iccid, uint32_t timeoutMs = 1000);
  bool getSignalQuality(int &rssi, int &ber, uint32_t timeoutMs = 1000);
  bool getRegistrationStatus(int &n, int &stat, uint32_t timeoutMs = 1000);
  bool isGprsAttached(bool &attached, uint32_t timeoutMs = 1000);
  bool setAPN(const char *apn, uint8_t cid = 1, uint32_t timeoutMs = 2000);
  bool activatePDP(uint8_t cid = 1, uint32_t timeoutMs = 10000);
  bool deactivatePDP(uint8_t cid = 1, uint32_t timeoutMs = 10000);

private:
  Stream &_serial;
  Stream *_debug;

  bool runCommand(const char *command,
                  const char *expect,
                  String *payload,
                  uint32_t timeoutMs);
  bool readLine(String &line, uint32_t timeoutMs);
  bool isTerminalLine(const String &line) const;
  void logLine(const char *prefix, const String &line);
  bool parseTwoIntegers(const String &line, const char *prefix, int &first, int &second);
  bool parseSingleInteger(const String &line, const char *prefix, int &value);
};

#endif
