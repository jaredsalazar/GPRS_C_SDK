#include <A9GHost.h>

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO)
#include <SoftwareSerial.h>
SoftwareSerial a9gSerial(7, 8);
#define A9G_SERIAL a9gSerial
#else
#define A9G_SERIAL Serial1
#endif

A9GHost modem(A9G_SERIAL);

void setup() {
  Serial.begin(115200);

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO)
  A9G_SERIAL.begin(115200);
#else
  A9G_SERIAL.begin(115200);
#endif

  modem.setDebug(&Serial);

  delay(2000);
  Serial.println("A9G basic probe");

  if (!modem.ping()) {
    Serial.println("Modem did not respond to AT");
    return;
  }

  modem.disableEcho();

  String imei;
  if (modem.getIMEI(imei)) {
    Serial.print("IMEI: ");
    Serial.println(imei);
  }

  String iccid;
  if (modem.getICCID(iccid)) {
    Serial.print("ICCID: ");
    Serial.println(iccid);
  }

  int rssi = 0;
  int ber = 0;
  if (modem.getSignalQuality(rssi, ber)) {
    Serial.print("CSQ RSSI: ");
    Serial.print(rssi);
    Serial.print(" BER: ");
    Serial.println(ber);
  }

  int n = 0;
  int stat = 0;
  if (modem.getRegistrationStatus(n, stat)) {
    Serial.print("CREG n=");
    Serial.print(n);
    Serial.print(" stat=");
    Serial.println(stat);
  }

  bool attached = false;
  if (modem.isGprsAttached(attached)) {
    Serial.print("GPRS attached: ");
    Serial.println(attached ? "yes" : "no");
  }
}

void loop() {
  delay(5000);
}
