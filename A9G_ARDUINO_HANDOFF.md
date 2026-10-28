# A9G Arduino Handoff

## Current status

This repo was used to build an Arduino-style workflow for the Ai-Thinker A9G dev board, but the work split across two locations:

- source repo in OneDrive:
  - `C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK`
- active no-space working copy used for real builds and flashing:
  - `C:\A9G\GPRS_C_SDK`
- toolkit:
  - `C:\A9G\CSDTK4.2`

The no-space copy matters because the vendor SDK/toolkit is fragile with spaces in paths.

## What definitely works

- native SDK build works from `C:\A9G\GPRS_C_SDK`
- Arduino IDE custom hardware package exists
- Arduino compile wrapper works
- CoolWatcher upload works
- fastpf flashing completes and verifies successfully on `COM20`
- the profile-selection popup is auto-dismissed, so upload is effectively one-click from Arduino IDE

## Important paths

- local Arduino hardware package:
  - `C:\Users\jared salazar\Documents\Arduino\hardware\ai-thinker\a9g`
- active wrapper tools:
  - `C:\A9G\arduino-a9g-tools`
- generated flash image:
  - `C:\A9G\GPRS_C_SDK\hex\app\app_flash.lod`

## Most important limitation

Flashing is proven. Runtime behavior on the real board is not fully proven.

We were able to:

- compile
- upload
- verify flash

But we were not able to reliably prove:

- visible onboard LED blinking
- external UART output from the custom app
- stable Arduino-style `setup()` / `loop()` behavior on the real board

## Hardware observations

- HST/download connection on `COM20` works with CoolWatcher
- external UART output seen on another port included:
  - `Init...`
  - `NO SIM CARD`
  - `+CIEV: "Charging",90%`
  - `+CREG: 2`

That output appears to be modem/system firmware output, not the custom app's Arduino `Serial` output.

## Current build state

The active no-space working copy is temporarily switched away from the full Arduino runtime and into a minimal native diagnostic build.

In the active working copy:

- [`app/Makefile`](C:\A9G\GPRS_C_SDK\app\Makefile) currently builds only:
  - `C_SRC := native_blink.c`
  - `CXX_SRC :=`

- current diagnostic source:
  - [`native_blink.c`](C:\A9G\GPRS_C_SDK\app\src\native_blink.c)

That diagnostic is currently a minimal native HST heartbeat app. It should:

- enable power domains
- feed the watchdog
- emit:
  - `native heartbeat start`
  - `native heartbeat tick=0`
  - `native heartbeat tick=1`
  - ...

once per second in CoolWatcher.

## Why the native diagnostic exists

We tried several layers of testing before pivoting:

- Arduino blink sketch
- Arduino serial sketch
- native LED blink tests on GPIO25, GPIO27, GPIO28
- native GPIO sweep across GPIO20-GPIO34

None gave a trustworthy visible onboard LED result on the real hardware.

Because of that, the debug strategy pivoted to the simplest possible native heartbeat over HST/CoolWatcher.

## Files that matter

OneDrive repo copies:

- [`ARDUINO_PORTING.md`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\ARDUINO_PORTING.md)
- [`A9G_ARDUINO_HANDOFF.md`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\A9G_ARDUINO_HANDOFF.md)
- [`app/Makefile`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\app\Makefile)
- [`arduino-package/ai-thinker/a9g/tools/a9g/upload_manual.bat`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\arduino-package\ai-thinker\a9g\tools\a9g\upload_manual.bat)
- [`arduino-package/ai-thinker/a9g/tools/a9g/a9g_arduino.rb`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\arduino-package\ai-thinker\a9g\tools\a9g\a9g_arduino.rb)
- [`arduino-package/ai-thinker/a9g/tools/a9g/dismiss_profile_selector.ps1`](C:\Users\jared salazar\OneDrive\Documents\GitHub\GPRS_C_SDK\arduino-package\ai-thinker\a9g\tools\a9g\dismiss_profile_selector.ps1)

Active no-space working copy:

- [`app/Makefile`](C:\A9G\GPRS_C_SDK\app\Makefile)
- [`native_blink.c`](C:\A9G\GPRS_C_SDK\app\src\native_blink.c)
- [`arduino_runtime.cpp`](C:\A9G\GPRS_C_SDK\app\src\arduino_runtime.cpp)
- [`sketch.cpp`](C:\A9G\GPRS_C_SDK\app\src\sketch.cpp)

## Upload flow

The current upload path uses:

- Arduino IDE local hardware package
- wrapper scripts
- CoolWatcher
- fastpf flash programmer

The helper flow now:

1. prepares the generated profile
2. launches CoolWatcher in the right working directory
3. auto-dismisses the profile window
4. auto-starts fastpf
5. flashes and verifies

## Current conclusion

The upload path is in good shape.

The unresolved problem is runtime validation on the board itself. At the time of handoff, the safest next step is not adding more Arduino features. It is verifying whether the native heartbeat stays alive after boot.

## Recommended next step

1. Flash the current native heartbeat app.
2. In CoolWatcher, stay on the Ruby Script output.
3. Confirm whether `native heartbeat tick=...` appears continuously once per second.

Interpretation:

- if heartbeat ticks continuously:
  - the app is alive
  - the LED problem is board-specific LED visibility/pin mapping
  - next step is restoring the Arduino runtime build and re-validating runtime behavior

- if heartbeat does not continue:
  - the app is not staying alive after boot
  - next step is debugging reset/runtime lifecycle, not Arduino APIs

## If restoring the Arduino runtime

Before continuing Arduino-layer work, remember that the active no-space copy was changed to native-only.

That means [`app/Makefile`](C:\A9G\GPRS_C_SDK\app\Makefile) will need to be restored from the broader Arduino runtime version so it again includes:

- `app/include`
- GPS headers
- C++ runtime sources
- `arduino_runtime.cpp`
- `sketch.cpp`
- the rest of the compatibility layer

## Practical recommendation

If the goal shifts away from forcing A9G itself to behave like a polished Arduino target, the most practical alternative is:

- use A9G as a modem/GNSS coprocessor over UART
- use a standard host MCU and higher-level environment on the host side

But if continuing the standalone-MCU effort, start with the native heartbeat proof before making more feature changes.
