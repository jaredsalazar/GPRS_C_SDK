#ifndef A9G_RUNTIME_H
#define A9G_RUNTIME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "api_inc_network.h"

#ifdef __cplusplus
extern "C" {
#endif

bool A9G_ReadAnalogChannel(uint8_t pin, uint16_t* rawValue, uint16_t* millivolts);

bool A9G_GpsBegin(void);
bool A9G_GpsEnd(void);
bool A9G_GpsIsOpen(void);
int A9G_GpsAvailable(void);
int A9G_GpsRead(void);
int A9G_GpsPeek(void);
void A9G_GpsFlush(void);

bool A9G_NetworkBegin(const char* apn, const char* userName, const char* userPasswd);
bool A9G_NetworkAttach(void);
bool A9G_NetworkActivate(void);
bool A9G_NetworkIsRegistered(void);
bool A9G_NetworkIsAttached(void);
bool A9G_NetworkIsActivated(void);
bool A9G_NetworkGetIp(char* ip, size_t size);
int A9G_NetworkGetSignalQuality(void);
Network_Status_t A9G_NetworkGetStatus(void);

#ifdef __cplusplus
}
#endif

#endif
