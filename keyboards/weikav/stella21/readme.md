# STELLA 21
[![STELLA 21](https://s21.ax1x.com/2025/02/19/pEM7NWD.jpg)](https://imgse.com/i/pEM7NWD)

* Keyboard Maintainer: [jonylee@hfd](https://github.com/jonylee1986)
* Hardware Supported: WEIKAV STELLA 21
* Hardware Availability: [WEIKAV](http://www.weikavcn.com)

Make example for this keyboard (after setting up your build environment):

    make weikav/stella21:default

Flashing example for this keyboard:

    make weikav/stella21:default:flash
    
See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 2 ways:

* **Bootmagic reset**: Hold down the Hold down the top left key (commonly programmed as *Esc*) and plug in the keyboard
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
