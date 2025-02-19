# Stars21
[![Stars21](https://s21.ax1x.com/2025/02/19/pEM73e1.jpg)](https://imgse.com/i/pEM73e1)

* Keyboard Maintainer: [jonylee@hfd](https://github.com/jonylee1986)
* Hardware Supported: WEIKAV Stars21
* Hardware Availability: [WEIKAV](http://www.weikavcn.com)

Make example for this keyboard (after setting up your build environment):

    make weikav/stars21:default

Flashing example for this keyboard:

    make weikav/stars21:default:flash
    
See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 2 ways:

* **Bootmagic reset**: Hold down the Hold down the top left key (commonly programmed as *Esc*) and plug in the keyboard
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
