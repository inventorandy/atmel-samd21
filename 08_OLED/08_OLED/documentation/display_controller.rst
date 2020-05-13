Display controller
==================

Display controller drivers are a part of the GFX Mono graphics library. They provide access to low-level functionality
of the controller and implement controller's protocol.

There can be any amount of instances of same or different display controller drivers in a system with the limitation by
available memory.

Architecture
------------

All display controller drivers are composed of two parts:

* Common interface

Common interface part contains interface that is shared between all display controllers. Common interface is a driver
for generic display controller. Each instance of particular display controller driver contains one instance of generic
display controller driver. This allows applications to use common interface of display controllers despite which display
controller is used in a system and simplifies switching to another display controllers supported in the GFX Mono library.

Common interface includes:

  * reading and writing data to/from display controller
  * setting page, column and start line addresses

* Controller specific

Each particular display controller driver also provides API specific to itself.

Provided functionality
----------------------

There are two levels of abstraction for functionality provided for the display controller driver:

* Command level
* Specific and commonly used functions

The command level provides generic API for commands and data sending to a display controller, it is a wrapper
around peripheral interface API which is used to communicate with the controller.

The subset of a display controller functions is extracted to separate API to provide more convenient way to use common
display controller functions.

There are two display controller currently supported by the GFX Mono library: SSD1306 and ST7565R.

Such functions for SSD1306:

* Hard reset
* Sleep control
* Turning on and off
* Contract adjustment
* Inversion of displayed pixels
* Clearing the whole display

Such functions for ST7565R:

* Hard and soft reset
* Sleep control
* Turning on and off
* Contract adjustment
* Inversion of displayed pixels
* Setting all display pixels on

GFX Mono library dependencies
-----------------------------
There is no need to create separate instances of the SSD1306 or ST7565R display controller driver when it is used as
part of the GFX Mono library since each display driver contains a instance of particular display controller.