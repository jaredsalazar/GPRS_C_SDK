#include "api_debug.h"
#include "api_hal_pm.h"
#include "api_hal_watchdog.h"
#include "api_os.h"

#define NATIVE_MAIN_TASK_STACK_SIZE (2048 * 2)
#define NATIVE_MAIN_TASK_PRIORITY   0
#define NATIVE_MAIN_TASK_NAME       "Native Blink"

static HANDLE g_nativeTaskHandle = NULL;

static void NativeBlinkTask(void *param)
{
    (void)param;
    uint32_t tick = 0;

    for (uint8_t i = 0; i < POWER_TYPE_MAX; ++i)
        PM_PowerEnable((Power_Type_t)i, true);

    Trace(1, "native heartbeat start");

    while (1)
    {
        WatchDog_KeepAlive();
        Trace(1, "native heartbeat tick=%d", tick++);
        OS_Sleep(1000);
    }
}

void app_Main(void)
{
    g_nativeTaskHandle = OS_CreateTask(
        NativeBlinkTask,
        NULL,
        NULL,
        NATIVE_MAIN_TASK_STACK_SIZE,
        NATIVE_MAIN_TASK_PRIORITY,
        0,
        0,
        NATIVE_MAIN_TASK_NAME);
    OS_SetUserMainHandle(&g_nativeTaskHandle);
}
