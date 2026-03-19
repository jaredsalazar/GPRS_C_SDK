# A9G Arduino Port Handoff

## Goal

Run Arduino-style sketches on the A9G itself as the standalone MCU, while using Arduino IDE as the authoring and build entrypoint.

This repo now contains:

- a native A9G Arduino-style runtime inside `app/`
- a local Arduino hardware package under `arduino-package/ai-thinker/a9g/`

It is not yet a finished Boards Manager package, but it is a working local custom platform foundation.

## Important External Paths

These are the no-space working copies that were used successfully:

- SDK copy: `C:\A9G\GPRS_C_SDK`
- Toolkit copy: `C:\A9G\CSDTK4.2`

Installed local Arduino package:

- `C:\Users\jared salazar\Documents\Arduino\hardware\ai-thinker\a9g`

The original toolkit/docs warned that paths with spaces break the vendor build. That was confirmed in practice, so the no-space copies matter.

## What Works

### Native A9G runtime in this repo

Files:

- `app/include/Arduino.h`
- `app/include/HardwareSerial.h`
- `app/include/Print.h`
- `app/include/Stream.h`
- `app/include/WString.h`
- `app/include/SPI.h`
- `app/include/Wire.h`
- `app/include/A9GGPS.h`
- `app/include/A9GNetwork.h`
- `app/include/A9GClient.h`
- `app/include/IPAddress.h`
- `app/include/Client.h`
- `app/include/Printable.h`
- `app/include/UDP.h`
- `app/include/A9GUDP.h`
- `app/include/A9GLocation.h`
- `app/src/arduino_runtime.cpp`
- `app/src/HardwareSerial.cpp`
- `app/src/Print.cpp`
- `app/src/Stream.cpp`
- `app/src/WString.cpp`
- `app/src/SPI.cpp`
- `app/src/Wire.cpp`
- `app/src/A9GGPS.cpp`
- `app/src/A9GNetwork.cpp`
- `app/src/A9GClient.cpp`
- `app/src/A9GUDP.cpp`
- `app/src/IPAddress.cpp`
- `app/src/A9GLocation.cpp`
- `app/src/A9GLocationBridge.c`
- `app/src/sketch.cpp`

Implemented currently:

- `pinMode`
- `digitalWrite`
- `digitalRead`
- `analogRead`
- `analogReadMilliVolts`
- `delay`
- `delayMicroseconds`
- `millis`
- `micros`
- `yield`
- `Serial`
- `SPI`
- `Wire`
- `Print`
- `Stream`
- `String`
- `GPS`
- `Cellular`
- `A9GClient`
- `IPAddress`
- `Printable`
- `UDP`
- `A9GUDP`
- `Location`

Notes:

- `HardwareSerial` now derives from `Stream`
- `TwoWire` now derives from `Stream`
- `SPI` is a minimal polling-mode wrapper over the native SDK SPI API
- `Wire` is a minimal master-mode wrapper over the native SDK I2C API
- `String` / `WString` is implemented minimally
- `analogRead` maps `A0` / `A1` to the SDK ADC channels and also exposes millivolts
- `GPS` is a buffered raw NMEA stream backed by `API_EVENT_ID_GPS_UART_RECEIVED`
- `Location` is a parsed GPS helper backed by the SDK GPS library state
- `Cellular` wraps the SDK GPRS attach/activate flow and auto-retries on the same event transitions used in the vendor network demo
- `A9GClient` is a minimal TCP client wrapper over the socket API
- `A9GUDP` is a minimal UDP sender/reader wrapper over the socket API
- `String` now has basic search/prefix/suffix/substring helpers and float constructors
- `HardwareSerial` now exposes `availableForWrite()`
- `Print` now supports `Printable`

### Local Arduino hardware package in this repo

Package root:

- `arduino-package/ai-thinker/a9g`

Key files:

- `boards.txt`
- `platform.txt`
- `programmers.txt`
- `variants/a9g_dev/pins_arduino.h`
- `cores/a9g/Arduino.h`
- `cores/a9g/HardwareSerial.h`
- `cores/a9g/Print.h`
- `cores/a9g/A9GGPS.h`
- `cores/a9g/A9GNetwork.h`
- `cores/a9g/A9GClient.h`
- `cores/a9g/IPAddress.h`
- `cores/a9g/Client.h`
- `cores/a9g/Printable.h`
- `cores/a9g/UDP.h`
- `cores/a9g/A9GUDP.h`
- `cores/a9g/A9GLocation.h`
- `cores/a9g/SPI.h`
- `cores/a9g/Stream.h`
- `cores/a9g/WString.h`
- `cores/a9g/Wire.h`
- `tools/a9g/compile_stub.bat`
- `tools/a9g/link_build.bat`
- `tools/a9g/finalize_build.bat`
- `tools/a9g/upload_manual.bat`
- `tools/a9g/a9g_arduino.rb`

What the wrapper currently does:

- stages builds into `C:\A9G\arduino-build\<project>\...`
- avoids mutating the canonical SDK tree during Arduino builds
- supports `.ino`
- supports extra sketch `.c/.cpp/.S`
- supports nested library-style `src/...` trees in staged builds
- hands upload off to CoolWatcher with a generated `a9g_arduino` profile

## Verified Builds

Native build from the no-space SDK copy succeeds with:

```bat
cmd /c "set GPRS_CSDTK42_PATH=C:\A9G\CSDTK4.2&& build.bat app"
```

Produced artifact:

- `C:\A9G\GPRS_C_SDK\hex\app\app_flash.lod`

There is still a recurring linker warning during packaging:

```text
mips-elf-ld.exe: hex/app/app.elf: access beyond end of merged section (-99878048)
```

This warning existed while successful `.lod` outputs were still generated.

## Current Upload State

Upload is improved but not fully hardware-verified.

The current upload flow:

- generates `C:\A9G\CSDTK4.2\cooltools\profiles\a9g_arduino.cwp`
- sets `C:\A9G\CSDTK4.2\cooltools\config.cfg` to open that profile
- copies `a9g_arduino.rb` into `C:\A9G\CSDTK4.2\cooltools\rbbase\a9g_arduino.rb`
- preloads:
  - the selected `.lod`
  - the 8955 flash programmer LOD
  - the selected COM port

This part was verified in no-launch mode, but actual flashing onto hardware was not verified because no physical A9G board was available.

## Files Synced Outside This Repo

These repo files were copied into:

- `C:\A9G\GPRS_C_SDK`
- `C:\Users\jared salazar\Documents\Arduino\hardware\ai-thinker\a9g`

This means the repo is the source of truth, but the current working local installation also depends on those external synced copies.

If another account continues the work, it should either:

1. keep using those same external paths, or
2. re-copy/sync the repo contents into fresh no-space locations

## Most Important Remaining Work

### 1. Hardware verification

Highest priority.

Needs:

- actual A9G board present
- test Arduino IDE compile
- test Arduino IDE upload
- confirm whether the generated CoolWatcher profile can auto-run `fastpfGo(...)`
- if auto-run fails, refine `upload_manual.bat` / `a9g_arduino.rb`

### 2. Hardware-backed feature validation

Important now that more APIs exist:

- verify `analogRead(A0/A1)` against real voltages
- verify GPS startup and confirm the buffered raw NMEA stream is populated
- verify parsed GPS values from `Location` against the raw NMEA output
- verify `Cellular.begin(apn, ...)` reaches attached + activated state and returns a valid IP
- verify `A9GClient` can open a TCP socket and receive data reliably
- verify `A9GUDP` can send and receive packets reliably

### 3. Broader Arduino compatibility

Most useful next code work:

- `IPAddress`
- optional socket/client wrapper on top of the activated PDP context
- better `A9GClient` buffering/timeout behavior
- better `A9GUDP` receive-side support and remote endpoint info
- more examples and quick-start docs

### 4. Boards Manager packaging

Not ready yet.

Still needed:

- final package structure cleanup
- archive layout
- `package_*.json`
- clean-install testing on another machine

## Practical Resume Plan

If continuing development later, the best order is:

1. verify the local A9G board appears in Arduino IDE
2. compile a very small sketch using `Serial`, `SPI`, and `Wire`
3. with hardware present, test upload
4. with hardware present, validate `analogRead`, `GPS`, and `Cellular`
5. after that, consider Boards Manager packaging

## Caution

Do not move the toolkit or working SDK back into paths with spaces.

That was an actual failure source, not just a documentation recommendation.
