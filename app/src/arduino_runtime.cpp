#include "Arduino.h"

#include "api_event.h"
#include "api_gps.h"
#include "api_network.h"
#include "A9GRuntime.h"
#include "gps.h"

#define ARDUINO_MAIN_TASK_STACK_SIZE  (2048 * 2)
#define ARDUINO_MAIN_TASK_PRIORITY    0
#define ARDUINO_MAIN_TASK_NAME        "Arduino Main"

#define ARDUINO_LOOP_TASK_STACK_SIZE  (2048 * 2)
#define ARDUINO_LOOP_TASK_PRIORITY    1
#define ARDUINO_LOOP_TASK_NAME        "Arduino Loop"

static HANDLE g_mainTaskHandle = NULL;
static HANDLE g_loopTaskHandle = NULL;
static HANDLE g_gpsMutex = NULL;
static HANDLE g_networkMutex = NULL;
static bool g_setupStarted = false;
static bool g_gpsLibraryInitialized = false;

static uint8_t g_gpsBuffer[512];
static uint16_t g_gpsHead = 0;
static uint16_t g_gpsTail = 0;
static bool g_networkContextValid = false;
static bool g_networkRegistered = false;
static bool g_networkAttached = false;
static bool g_networkActivated = false;
static int g_signalQuality = -1;
static Network_Status_t g_networkStatus = NETWORK_STATUS_OFFLINE;
static Network_PDP_Context_t g_pdpContext;

static GPIO_LEVEL toGpioLevel(uint8_t value)
{
    return value == HIGH ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW;
}

static void EnsureRuntimeState(void)
{
    if (g_gpsMutex == NULL)
        g_gpsMutex = OS_CreateMutex();
    if (g_networkMutex == NULL)
        g_networkMutex = OS_CreateMutex();
}

static int analogPinToChannel(uint8_t pin)
{
    switch (pin)
    {
        case A0:
            return ADC_CHANNEL_0;
        case A1:
            return ADC_CHANNEL_1;
        default:
            break;
    }

    if (pin < ADC_CHANNEL_MAX)
        return pin;

    return -1;
}

static void copyPdpField(char* destination, size_t destinationSize, const char* source)
{
    size_t index = 0;

    if (destination == NULL || destinationSize == 0)
        return;

    if (source == NULL)
        source = "";

    while (index + 1 < destinationSize && source[index] != '\0')
    {
        destination[index] = source[index];
        ++index;
    }

    destination[index] = '\0';
}

static bool NetworkEnsureConnectionLocked(void)
{
    uint8_t status = 0;

    if (!g_networkContextValid)
        return false;

    if (!Network_GetAttachStatus(&status))
        return false;

    if (!status)
        return Network_StartAttach();

    if (!Network_GetActiveStatus(&status))
        return false;

    if (!status)
        return Network_StartActive(g_pdpContext);

    return true;
}

static void GpsPushBytes(const uint8_t* data, uint32_t length)
{
    uint32_t index = 0;

    if (data == NULL || length == 0)
        return;

    EnsureRuntimeState();
    if (g_gpsMutex == NULL)
        return;

    OS_LockMutex(g_gpsMutex);
    while (index < length)
    {
        uint16_t next = (uint16_t)((g_gpsHead + 1) % sizeof(g_gpsBuffer));
        if (next == g_gpsTail)
            g_gpsTail = (uint16_t)((g_gpsTail + 1) % sizeof(g_gpsBuffer));
        g_gpsBuffer[g_gpsHead] = data[index++];
        g_gpsHead = next;
    }
    OS_UnlockMutex(g_gpsMutex);
}

void pinMode(uint8_t pin, uint8_t mode)
{
    GPIO_config_t config;
    config.pin = (GPIO_PIN)pin;
    config.mode = mode == OUTPUT ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT;
    config.defaultLevel = GPIO_LEVEL_LOW;
    config.intConfig.debounce = 0;
    config.intConfig.type = GPIO_INT_TYPE_MAX;
    config.intConfig.callback = NULL;
    GPIO_Init(config);
}

void digitalWrite(uint8_t pin, uint8_t value)
{
    GPIO_Set((GPIO_PIN)pin, toGpioLevel(value));
}

int digitalRead(uint8_t pin)
{
    GPIO_LEVEL level = GPIO_LEVEL_LOW;
    if (!GPIO_Get((GPIO_PIN)pin, &level))
        return LOW;
    return level == GPIO_LEVEL_HIGH ? HIGH : LOW;
}

int analogRead(uint8_t pin)
{
    uint16_t rawValue = 0;
    uint16_t millivolts = 0;

    if (!A9G_ReadAnalogChannel(pin, &rawValue, &millivolts))
        return -1;

    return rawValue;
}

int analogReadMilliVolts(uint8_t pin)
{
    uint16_t rawValue = 0;
    uint16_t millivolts = 0;

    if (!A9G_ReadAnalogChannel(pin, &rawValue, &millivolts))
        return -1;

    return millivolts;
}

void delay(uint32_t ms)
{
    OS_Sleep(ms);
}

void delayMicroseconds(uint32_t us)
{
    OS_SleepUs(us);
}

unsigned long millis(void)
{
    return (unsigned long)(clock() / CLOCKS_PER_MSEC);
}

unsigned long micros(void)
{
    return (unsigned long)((clock() * 1000000UL) / CLOCKS_PER_SEC);
}

void yield(void)
{
}

extern "C" bool A9G_ReadAnalogChannel(uint8_t pin, uint16_t* rawValue, uint16_t* millivolts)
{
    ADC_Config_t config;
    uint16_t localRaw = 0;
    uint16_t localMv = 0;
    int channel = analogPinToChannel(pin);

    if (channel < 0)
        return false;

    config.channel = (ADC_Channel_t)channel;
    config.samplePeriod = ADC_SAMPLE_PERIOD_1MS;
    ADC_Init(config);
    if (!ADC_Read((ADC_Channel_t)channel, &localRaw, &localMv))
    {
        ADC_Close((ADC_Channel_t)channel);
        return false;
    }

    ADC_Close((ADC_Channel_t)channel);

    if (rawValue != NULL)
        *rawValue = localRaw;
    if (millivolts != NULL)
        *millivolts = localMv;

    return true;
}

extern "C" bool A9G_GpsBegin(void)
{
    EnsureRuntimeState();
    if (!g_gpsLibraryInitialized)
    {
        GPS_Init();
        g_gpsLibraryInitialized = true;
    }
    return GPS_IsOpen() ? true : GPS_Open(NULL);
}

extern "C" bool A9G_GpsEnd(void)
{
    return GPS_Close();
}

extern "C" bool A9G_GpsIsOpen(void)
{
    return GPS_IsOpen();
}

extern "C" int A9G_GpsAvailable(void)
{
    int available = 0;

    EnsureRuntimeState();
    if (g_gpsMutex == NULL)
        return 0;

    OS_LockMutex(g_gpsMutex);
    if (g_gpsHead >= g_gpsTail)
        available = g_gpsHead - g_gpsTail;
    else
        available = (int)(sizeof(g_gpsBuffer) - g_gpsTail + g_gpsHead);
    OS_UnlockMutex(g_gpsMutex);
    return available;
}

extern "C" int A9G_GpsRead(void)
{
    int value = -1;

    EnsureRuntimeState();
    if (g_gpsMutex == NULL)
        return -1;

    OS_LockMutex(g_gpsMutex);
    if (g_gpsHead != g_gpsTail)
    {
        value = g_gpsBuffer[g_gpsTail];
        g_gpsTail = (uint16_t)((g_gpsTail + 1) % sizeof(g_gpsBuffer));
    }
    OS_UnlockMutex(g_gpsMutex);
    return value;
}

extern "C" int A9G_GpsPeek(void)
{
    int value = -1;

    EnsureRuntimeState();
    if (g_gpsMutex == NULL)
        return -1;

    OS_LockMutex(g_gpsMutex);
    if (g_gpsHead != g_gpsTail)
        value = g_gpsBuffer[g_gpsTail];
    OS_UnlockMutex(g_gpsMutex);
    return value;
}

extern "C" void A9G_GpsFlush(void)
{
    EnsureRuntimeState();
    if (g_gpsMutex == NULL)
        return;

    OS_LockMutex(g_gpsMutex);
    g_gpsHead = 0;
    g_gpsTail = 0;
    OS_UnlockMutex(g_gpsMutex);
}

extern "C" bool A9G_NetworkBegin(const char* apn, const char* userName, const char* userPasswd)
{
    bool result = false;

    EnsureRuntimeState();
    if (g_networkMutex == NULL)
        return false;

    OS_LockMutex(g_networkMutex);
    copyPdpField(g_pdpContext.apn, sizeof(g_pdpContext.apn), apn);
    copyPdpField(g_pdpContext.userName, sizeof(g_pdpContext.userName), userName);
    copyPdpField(g_pdpContext.userPasswd, sizeof(g_pdpContext.userPasswd), userPasswd);
    g_networkContextValid = true;
    result = NetworkEnsureConnectionLocked();
    OS_UnlockMutex(g_networkMutex);
    return result;
}

extern "C" bool A9G_NetworkAttach(void)
{
    EnsureRuntimeState();
    if (g_networkMutex == NULL)
        return false;

    OS_LockMutex(g_networkMutex);
    bool result = Network_StartAttach();
    OS_UnlockMutex(g_networkMutex);
    return result;
}

extern "C" bool A9G_NetworkActivate(void)
{
    bool result = false;

    EnsureRuntimeState();
    if (g_networkMutex == NULL)
        return false;

    OS_LockMutex(g_networkMutex);
    result = NetworkEnsureConnectionLocked();
    OS_UnlockMutex(g_networkMutex);
    return result;
}

extern "C" bool A9G_NetworkIsRegistered(void)
{
    return g_networkRegistered;
}

extern "C" bool A9G_NetworkIsAttached(void)
{
    return g_networkAttached;
}

extern "C" bool A9G_NetworkIsActivated(void)
{
    return g_networkActivated;
}

extern "C" bool A9G_NetworkGetIp(char* ip, size_t size)
{
    if (ip == NULL || size == 0 || size > 255)
        return false;

    return Network_GetIp(ip, (uint8_t)size);
}

extern "C" int A9G_NetworkGetSignalQuality(void)
{
    return g_signalQuality;
}

extern "C" Network_Status_t A9G_NetworkGetStatus(void)
{
    return g_networkStatus;
}

static void ArduinoLoopTask(void* param)
{
    (void)param;

    setup();
    while (1)
    {
        loop();
        yield();
    }
}

static void StartArduinoSketch(void)
{
    if (g_setupStarted)
        return;

    g_setupStarted = true;
    g_loopTaskHandle = OS_CreateTask(ArduinoLoopTask,
        NULL, NULL, ARDUINO_LOOP_TASK_STACK_SIZE, ARDUINO_LOOP_TASK_PRIORITY, 0, 0, ARDUINO_LOOP_TASK_NAME);
}

static void HandleEvent(API_Event_t* event)
{
    if (event == NULL)
        return;

    switch (event->id)
    {
        case API_EVENT_ID_SYSTEM_READY:
            StartArduinoSketch();
            break;
        case API_EVENT_ID_GPS_UART_RECEIVED:
            if (event->pParam1 != NULL && event->param1 > 0)
                GPS_Update(event->pParam1, event->param1);
            GpsPushBytes(event->pParam1, event->param1);
            break;
        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            g_networkRegistered = true;
            g_networkStatus = NETWORK_STATUS_REGISTERED;
            A9G_NetworkActivate();
            break;
        case API_EVENT_ID_NETWORK_REGISTER_SEARCHING:
            g_networkStatus = NETWORK_STATUS_REGISTERING;
            break;
        case API_EVENT_ID_NETWORK_REGISTER_DENIED:
        case API_EVENT_ID_NETWORK_REGISTER_NO:
            g_networkRegistered = false;
            g_networkAttached = false;
            g_networkActivated = false;
            g_networkStatus = NETWORK_STATUS_OFFLINE;
            break;
        case API_EVENT_ID_NETWORK_DETACHED:
            g_networkAttached = false;
            g_networkActivated = false;
            g_networkStatus = NETWORK_STATUS_DETACHED;
            A9G_NetworkActivate();
            break;
        case API_EVENT_ID_NETWORK_ATTACH_FAILED:
            g_networkAttached = false;
            g_networkActivated = false;
            g_networkStatus = NETWORK_STATUS_ATTACH_FAILED;
            A9G_NetworkActivate();
            break;
        case API_EVENT_ID_NETWORK_ATTACHED:
            g_networkAttached = true;
            g_networkStatus = NETWORK_STATUS_ATTACHED;
            A9G_NetworkActivate();
            break;
        case API_EVENT_ID_NETWORK_DEACTIVED:
            g_networkActivated = false;
            g_networkStatus = NETWORK_STATUS_DEACTIVED;
            A9G_NetworkActivate();
            break;
        case API_EVENT_ID_NETWORK_ACTIVATE_FAILED:
            g_networkActivated = false;
            g_networkStatus = NETWORK_STATUS_ACTIVATE_FAILED;
            A9G_NetworkActivate();
            break;
        case API_EVENT_ID_NETWORK_ACTIVATED:
            g_networkAttached = true;
            g_networkActivated = true;
            g_networkStatus = NETWORK_STATUS_ACTIVATED;
            break;
        case API_EVENT_ID_SIGNAL_QUALITY:
            g_signalQuality = (int)event->param1;
            break;
        default:
            break;
    }
}

static void ArduinoMainTask(void* param)
{
    (void)param;

    API_Event_t* event = NULL;

    for (uint8_t i = 0; i < POWER_TYPE_MAX; ++i)
        PM_PowerEnable((Power_Type_t)i, true);

    while (1)
    {
        if (OS_WaitEvent(g_mainTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            HandleEvent(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

extern "C" void app_Main(void)
{
    EnsureRuntimeState();
    g_mainTaskHandle = OS_CreateTask(ArduinoMainTask,
        NULL, NULL, ARDUINO_MAIN_TASK_STACK_SIZE, ARDUINO_MAIN_TASK_PRIORITY, 0, 0, ARDUINO_MAIN_TASK_NAME);
    OS_SetUserMainHandle(&g_mainTaskHandle);
}
