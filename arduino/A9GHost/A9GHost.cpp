#include "A9GHost.h"

A9GHost::A9GHost(Stream &serial) : _serial(serial), _debug(NULL) {}

void A9GHost::setDebug(Stream *debug) {
  _debug = debug;
}

void A9GHost::flushInput() {
  while (_serial.available()) {
    _serial.read();
  }
}

bool A9GHost::ping(uint32_t timeoutMs) {
  return runCommand("AT", "OK", NULL, timeoutMs);
}

bool A9GHost::disableEcho(uint32_t timeoutMs) {
  return runCommand("ATE0", "OK", NULL, timeoutMs);
}

bool A9GHost::getIMEI(String &imei, uint32_t timeoutMs) {
  String payload;
  if (!runCommand("AT+CGSN", "OK", &payload, timeoutMs)) {
    return false;
  }

  payload.trim();
  if (payload.length() == 0) {
    return false;
  }

  imei = payload;
  return true;
}

bool A9GHost::getICCID(String &iccid, uint32_t timeoutMs) {
  String payload;
  if (!runCommand("AT+CCID", "OK", &payload, timeoutMs)) {
    return false;
  }

  payload.trim();
  if (payload.startsWith("+CCID:")) {
    payload.remove(0, 6);
  }
  payload.trim();
  if (payload.length() == 0) {
    return false;
  }

  iccid = payload;
  return true;
}

bool A9GHost::getSignalQuality(int &rssi, int &ber, uint32_t timeoutMs) {
  String payload;
  if (!runCommand("AT+CSQ", "OK", &payload, timeoutMs)) {
    return false;
  }
  return parseTwoIntegers(payload, "+CSQ:", rssi, ber);
}

bool A9GHost::getRegistrationStatus(int &n, int &stat, uint32_t timeoutMs) {
  String payload;
  if (!runCommand("AT+CREG?", "OK", &payload, timeoutMs)) {
    return false;
  }
  return parseTwoIntegers(payload, "+CREG:", n, stat);
}

bool A9GHost::isGprsAttached(bool &attached, uint32_t timeoutMs) {
  String payload;
  int value = 0;

  if (!runCommand("AT+CGATT?", "OK", &payload, timeoutMs)) {
    return false;
  }
  if (!parseSingleInteger(payload, "+CGATT:", value)) {
    return false;
  }

  attached = (value == 1);
  return true;
}

bool A9GHost::setAPN(const char *apn, uint8_t cid, uint32_t timeoutMs) {
  String command = "AT+CGDCONT=";
  command += cid;
  command += ",\"IP\",\"";
  command += apn;
  command += "\"";
  return runCommand(command.c_str(), "OK", NULL, timeoutMs);
}

bool A9GHost::activatePDP(uint8_t cid, uint32_t timeoutMs) {
  String command = "AT+CGACT=1,";
  command += cid;
  return runCommand(command.c_str(), "OK", NULL, timeoutMs);
}

bool A9GHost::deactivatePDP(uint8_t cid, uint32_t timeoutMs) {
  String command = "AT+CGACT=0,";
  command += cid;
  return runCommand(command.c_str(), "OK", NULL, timeoutMs);
}

bool A9GHost::runCommand(const char *command,
                         const char *expect,
                         String *payload,
                         uint32_t timeoutMs) {
  flushInput();
  _serial.print(command);
  _serial.print("\r\n");

  if (_debug != NULL) {
    _debug->print(">> ");
    _debug->println(command);
  }

  String combinedPayload;
  unsigned long deadline = millis() + timeoutMs;

  while ((long)(deadline - millis()) >= 0) {
    String line;
    if (!readLine(line, timeoutMs)) {
      continue;
    }
    line.trim();
    if (line.length() == 0) {
      continue;
    }
    if (line == command) {
      continue;
    }

    logLine("<< ", line);

    if (line == expect) {
      if (payload != NULL) {
        *payload = combinedPayload;
      }
      return true;
    }

    if (line == "ERROR" || line.startsWith("+CME ERROR") || line.startsWith("+CMS ERROR")) {
      return false;
    }

    if (!isTerminalLine(line)) {
      if (combinedPayload.length() > 0) {
        combinedPayload += '\n';
      }
      combinedPayload += line;
    }
  }

  return false;
}

bool A9GHost::readLine(String &line, uint32_t timeoutMs) {
  unsigned long deadline = millis() + timeoutMs;
  line = "";

  while ((long)(deadline - millis()) >= 0) {
    while (_serial.available()) {
      char ch = (char)_serial.read();
      if (ch == '\r') {
        continue;
      }
      if (ch == '\n') {
        if (line.length() > 0) {
          return true;
        }
        continue;
      }
      line += ch;
    }
    delay(1);
  }

  return line.length() > 0;
}

bool A9GHost::isTerminalLine(const String &line) const {
  return line == "OK" || line == "ERROR" || line.startsWith("+CME ERROR") ||
         line.startsWith("+CMS ERROR");
}

void A9GHost::logLine(const char *prefix, const String &line) {
  if (_debug == NULL) {
    return;
  }
  _debug->print(prefix);
  _debug->println(line);
}

bool A9GHost::parseTwoIntegers(const String &line,
                               const char *prefix,
                               int &first,
                               int &second) {
  if (!line.startsWith(prefix)) {
    return false;
  }

  String work = line.substring(strlen(prefix));
  work.trim();
  int comma = work.indexOf(',');
  if (comma < 0) {
    return false;
  }

  first = work.substring(0, comma).toInt();
  second = work.substring(comma + 1).toInt();
  return true;
}

bool A9GHost::parseSingleInteger(const String &line, const char *prefix, int &value) {
  if (!line.startsWith(prefix)) {
    return false;
  }

  String work = line.substring(strlen(prefix));
  work.trim();
  value = work.toInt();
  return true;
}
