# Arduino Porting Assessment

## Short answer

This repository does not contain enough portable implementation detail to move the full A9G C SDK onto the Arduino platform as a direct source-level port.

What it does contain is enough information to build an Arduino host library that controls an A9G module over UART. That is the practical path, and an initial version is included under `arduino/A9GHost`.

## Why the full SDK is not directly portable

The SDK application code is linked against the A9G firmware runtime, not against normal portable C implementations.

Examples:

- [`include/sdk_init.h`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\include\sdk_init.h) exposes nearly every API as a function pointer inside `T_INTERFACE_VTBL_TAG`.
- [`include/sdk_init.h`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\include\sdk_init.h) defines `CSDK_FUNC(name)` as `g_InterfaceVtbl->name`.
- [`init/src/sdk_init.c`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\init\src\sdk_init.c) shows that the firmware injects this table at runtime through `user_Init`.
- The repo also depends on vendor binaries, linker setup, and the original target toolchain under `platform/`.

That means Arduino cannot compile and run this SDK "as-is" because Arduino boards do not provide:

- the A9G firmware callback table
- the A9G OS/task/event model
- the A9G modem/network stack
- the target memory layout and startup model
- the MIPS/RDA8955 toolchain and binary interface

## What is reusable

These parts are reasonably reusable or adaptable:

- `libs/gps/minmea`: portable NMEA parser
- `libs/cjson`: portable JSON library
- selected small utility code after cleanup
- high-level examples as behavioral references

These parts are not directly portable:

- `include/api_*` APIs backed by `CSDK_FUNC(...)`
- task/event code using `OS_CreateTask`, `OS_WaitEvent`, etc.
- network/socket/GPRS functions implemented by the A9G firmware
- board power, GPIO, UART, flash, PM, audio, and internal GPS control APIs

## Practical Arduino strategy

Use Arduino as the host MCU and the A9G as an external modem/GPS module over UART:

1. Arduino sends AT commands to A9G.
2. A9G handles GSM/GPRS/GNSS internally.
3. Arduino reads responses and optional NMEA output.

This matches how Arduino typically integrates cellular modules and avoids trying to recreate the proprietary runtime from this SDK.

## Included adaptation

An initial Arduino library is provided here:

- [`arduino/A9GHost/A9GHost.h`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\arduino\A9GHost\A9GHost.h)
- [`arduino/A9GHost/A9GHost.cpp`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\arduino\A9GHost\A9GHost.cpp)
- [`arduino/A9GHost/library.properties`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\arduino\A9GHost\library.properties)
- [`arduino/A9GHost/examples/BasicInfo/BasicInfo.ino`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\arduino\A9GHost\examples\BasicInfo\BasicInfo.ino)

Current scope:

- modem ping
- echo disable
- IMEI read
- ICCID read
- signal quality read
- network registration query
- GPRS attach query
- APN configuration
- PDP activation and deactivation

There is also now a native A9G Arduino-style runtime for the standalone-MCU case:

- [`app/include/Arduino.h`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\app\include\Arduino.h)
- [`app/include/HardwareSerial.h`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\app\include\HardwareSerial.h)
- [`app/src/arduino_runtime.cpp`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\app\src\arduino_runtime.cpp)
- [`app/src/sketch.cpp`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\app\src\sketch.cpp)

That layer provides:

- `setup()` and `loop()`
- `pinMode`, `digitalWrite`, `digitalRead`
- `delay`, `delayMicroseconds`, `millis`, `micros`
- `Serial` on `UART1`
- `Serial2` on `UART2`

## Building the Arduino-style standalone app

The repository still defaults to one of the original demos in the root `Makefile`.

Build the Arduino-style app explicitly:

```bat
build.bat app
```

Your sketch entry points are:

- [`app/src/sketch.cpp`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\app\src\sketch.cpp)
- [`app/src/arduino_runtime.cpp`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\app\src\arduino_runtime.cpp)

Today this is "Arduino-like source compatibility inside the native SDK", not a full Arduino board package.

## What is still missing for an official Arduino port

To work as a real Arduino board inside Arduino IDE or Arduino CLI, you would still need all of the following:

- a packaged board core directory with Arduino platform metadata
- `boards.txt` definitions for one or more A9/A9G board variants
- `platform.txt` recipes that invoke the A9G toolchain and linker correctly
- packaged tool dependencies so Arduino IDE can install or locate the A9G compiler and utilities
- upload recipes wired to the actual A9G flashing/downloader flow
- variant files mapping Arduino pin numbers to A9G GPIOs
- a more complete core API surface such as `Stream`, `Print`, interrupts, `analogRead`, `SPI`, `Wire`, and timing semantics
- example sketches and board documentation
- regression testing to prove builds, uploads, serial, GPIO, timing, and library compatibility

In other words, the current work makes writing Arduino-style code possible inside this SDK tree. An official Arduino port would package that work into the structure and metadata that Arduino tooling expects.

## What would be needed for a larger Arduino port

If you want a fuller Arduino library, the next layer would be:

- SMS helpers
- HTTP/TCP helpers over AT commands
- GPS enable/query helpers based on confirmed A9G AT commands
- optional integration of `libs/gps/minmea` for sentence parsing

For that work, the missing piece is not this SDK source tree. The missing piece is a reliable A9G AT command reference for the exact firmware running on your module.
