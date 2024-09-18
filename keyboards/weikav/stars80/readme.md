# Stars80
[![Stars80](https://s21.ax1x.com/2024/03/19/pFRg3cT.png)](https://imgse.com/i/pFRg3cT)

A customizable 80% multimodal keyboard.

* Keyboard Maintainer: [jonylee@hfd](https://github.com/jonylee1986)
* Hardware Supported: WEIKAV Stars80
* Hardware Availability: [WEIKAV](http://www.weikavcn.com)

Make example for this keyboard (after setting up your build environment):

    make weikav/stars80:default

Flashing example for this keyboard:

    make weikav/stars80:default:flash
    
See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 2 ways:

* **Bootmagic reset**: Hold down the Hold down the top left key (commonly programmed as *Esc*) and plug in the keyboard
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
