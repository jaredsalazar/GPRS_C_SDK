# A9G Arduino Hardware Package

This is a local Arduino IDE hardware package for the Ai-Thinker A9G.

## Current status

This package is a wrapper around the native A9G SDK build system.

- Arduino IDE can discover the board from the sketchbook `hardware` folder.
- Build delegates to `build.bat app` inside an isolated generated workspace under `C:\A9G\arduino-build`.
- Upload is still manual through CoolWatcher.
- ADC, raw GPS, parsed GPS, GPRS attach/activate, simple TCP client helpers, and a minimal UDP wrapper are available in the core.

## Current assumptions

- SDK path: `C:\A9G\GPRS_C_SDK`
- Toolkit path: `C:\A9G\CSDTK4.2`
- Windows host

## Current limitations

- Intended for ordinary `.ino` sketches
- Not yet a Boards Manager package
- Not yet a fully native Arduino toolchain integration
- Upload is not automated
- Supports the main `.ino` sketch plus extra `.c/.cpp/.S` files
- Preserves staged library folder structure for many libraries that keep sources under a `src/` tree
- Complex libraries that rely on advanced Arduino build flags, generated sources, or architecture-specific selection may still need more work

## Required runtime in the SDK

The native SDK app should contain the Arduino-style runtime and serial wrapper already added under:

- `app/include`
- `app/src`

## Current built-in examples

- `examples/ADCRead`
- `examples/GPSRawNMEA`
- `examples/GPSParsed`
- `examples/GPRSHttp`
- `examples/GPRSUdp`
