# Proposed refactor

The goal is to convert the repository into an Arduino library. Info on how to create an Arduino library can be found in the [documentation](https://docs.arduino.cc/learn/contributions/arduino-creating-library-guide/?_gl=1*1p7rvq6*_up*MQ..*_ga*MjUzMTA4OTYyLjE3OTE0NTExNTI.*_ga_NEXN8H46L5*czE3OTE0NTU3OTAkbzIkZzAkdDE3OTE0NTU3OTAkajYwJGwwJGgzNjg2MTUzOTA.).

Usually, the sketches are kept in an `examples/` folder, and the rest of the code goes in the root directory (or in a sub-directory, as we propose here).

This is the general current structure, with each sensor in separate directory with its own sub-directory `src`, which is largely the same between sensors.

```text
.
├── wari_3G/
│   ├── src/
│   │   ├── Rio_COAP.cpp
│   │   ├── Rio_COAP.h
│   │   ├── ...
│   │   └── XBee_dev.h
│   ├── Readme.md
│   └── wari_3G.ino
│
├── wari_3G_v2/
│   ├── src/
│   │   ├── Rio_COAP.cpp
│   │   ├── Rio_COAP.h
│   │   ├── ...
│   │   └── XBee_dev.h
│   ├── Readme.md
│   └── wari_3G_v2.ino
│
├── ...
│
├── wari_lidar_lora/
│   ├── src/
│   │   ├── Rio_COAP.cpp
│   │   ├── Rio_COAP.h
│   │   └── ...
│   ├── Readme.md
│   └── wari_lidar_lora.ino
│
└── platformio.ini
```

To refactor into an Arduino library structure, we would make the following changes:

- Make `src/` files consistent between sensors (where possible, leaving out `wari_lidar_lora`)
  - As outlined by Kaiyode in #37
  - Move `src/` into the root directory
- Move per-board settings into a config file for each sensor
- Sensor sketches go into the `examples` folder (in their own similarly named directory, as per Arduino-style)
- `wari_lidar_lora` still has its own `src` folder
- Sensors use `#include "Rio.h"` instead of `#include "src/Rio.h"` (except for `wari_lidar_lora`)
- In Arduino, the repo is copied into the libraries folder (on my PC, this is `~/Documents/Arduino/libraries`), so it is recognised as an installed library
  - Users will pull into this repo when there are changes upstream
  - Users open the sketches within the examples folder when using the IDE
- A [`library.properties`](https://docs.arduino.cc/arduino-cli/library-specification/) file is included, so Arduino recognises this as a library

Proposed structure:

```text
.
├── src/
│   ├── Rio_COAP.cpp
│   ├── Rio_COAP.h
│   ├── ...
│   └── XBee_dev.h
│
├── examples/
│   ├── wari_3G/
│   │   ├── wari_3G.ino
│   │   ├── Readme.md
│   │   └── config.h
│   ├── wari_3G_v2/
│   │   ├── wari_3G_v2.ino
│   │   ├── Readme.md
│   │   └── config.h
│   ├── ...
│   └── wari_lidar_lora/
│       ├── src/
│       │   ├── Rio_COAP.cpp
│       │   ├── Rio_COAP.h
│       │   └── ...
│       ├── wari_lidar_lora.ino
│       └── Readme.md
│
├── platformio.ini
└── library.properties
```

## PlatformIO configuration

So that this still works in PlatformIO, the `platformio.ini` requires some changes:

- We add `build_flags = -I src` to specify the include path (see [documentation](https://docs.platformio.org/en/stable/projectconf/sections/env/options/build/build_flags.html) for more info)
- Add `build_src_filter` to all sensors to ensure it doesn't get confused between the different copies of `src` (for `wari_lidar_lora`)

For example:

```text
[platformio]
src_dir = .

[common]
platform = atmelavr
board = pro8MHzatmega328
framework = arduino
lib_deps =
  makuna/RTC
  greiman/SdFat
  rocketscream/Low-Power
  paulstoffregen/AltSoftSerial
  garmin/LIDAR-Lite
  marzogh/SPIMemory
check_tool = clangtidy
check_flags = clangtidy: --config-file=.clang-tidy
extra_scripts = pre:find_ino_files.py
check_skip_packages = yes
build_flags = -I src

[extended_libs]
lib_deps =
  ${common.lib_deps}
  paulstoffregen/AltSoftSerial
  garmin/LIDAR-Lite
  marzogh/SPIMemory

[env:wari_3G]
extends = common
lib_deps = ${extended_libs.lib_deps}
custom_sensor_dir = examples/wari_3G
check_src_filters =
  +<wari_3G/*>
  +<src/*>
  -<src/XBee_dev*>
build_src_filter =
  -<*>
  +<examples/wari_3G/*>
  +<src/*>

...

[env:wari_lidar_lora]
extends = common
lib_deps = ${extended_libs.lib_deps}
custom_sensor_dir = examples/wari_lidar_lora
check_src_filters =
  +<examples/wari_lidar_lora/*>
  -<examples/wari_lidar_lora/src/XBee_dev*>
build_src_filter =
  -<*>
  +<examples/wari_lidar_lora/*>
```
