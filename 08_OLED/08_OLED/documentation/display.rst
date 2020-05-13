Display
=======

Display drivers are a part of the GFX Mono graphics library. They provide basic graphic primitives to a user and the
GFX Mono driver.

There can be any amount of instances of same or different display drivers in a system with the limitation by available
memory.

Architecture and provided functionality
---------------------------------------

Display drivers provide API to work with pixels, bytes and pages of a display:

* A pixel is the smallest addressable element of a display, that can be read or written.
* It is possible to control 8 sequential pixels via writing, reading or masking a byte.
* Page is an area of 8 pixel height and display width; it can be read or written.

All display drivers are composed of two parts:

* Common interface

Common interface part contains interface that is shared between all display drivers. Common interface is a driver
for a generic display. Each instance of particular display driver contains one instance of generic display controller.
This allows applications to use common interface of display despite which display is used in a system and simplifies
switching to another display supported in the GFX Mono library.

Common interface includes:

  * drawing a pixel
  * drawing 8 pixels via sending or masking a byte
  * drawing a page on a display

* Display specific

Additional features
-------------------

Frame buffer
^^^^^^^^^^^^
Some displays may have no readable internal memory, making it impossible to read the value of pixels. If an application
requires the possibility to read values back from a display, frame buffer can be enabled to achieve this.

Frame buffer is a buffer allocated in RAM, and all write-to-display operations are automatically doubled in it.
The GFX Mono library requires it.

Null display
^^^^^^^^^^^^
The null display driver is a frame buffer used a display. It provides the same functionality as a regular display,
but all graphics is kept in RAM only. It can be useful for debugging purposes.

GFX Mono library dependencies
-----------------------------
* An instance of the display driver must be passed to each instance of the GFX Mono driver.
* An instance of an IO must be passed to each instance of the display driver.
