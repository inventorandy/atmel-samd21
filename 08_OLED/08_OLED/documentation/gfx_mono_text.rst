GFX Mono Text
=============

GFX Mono Text driver is a part of the GFX Mono graphics library, it provides means to draw symbols, strings and bounding
boxes.

There can be any amount of instances of the GFX Mono Text driver in a system with the limitation by available memory.
Each instance requires an instance of the GFX Mono drvier driver to operate. Each instance of the GFX Mono can work with
only one physical display; if there are two displays, two instances of the GFX Mono must be created.

Used font is not pre-defined and must be passed to the driver each time symbol or string is drawn. This makes it
possible to support any number of fonts and simplifies new font support.

There are 4 predefined fonts available in the GFX Mono library. All fonts are localed in non-volatile memory.
