# RGB-LED-API

This is an Arduino sketch to drive an RGB LED strip in response to HTTP calls.

## Current state

Can currently only handle `POST` requests that are sent via `curl -X POST {location}/{color}`, the color can be in the form of a hexcode `0xFFFFFF` or one of the following colors, all case insensitive.

This program can only handle _static_ changes to the LEDs. IE it cannot create fading effects for the LED strip. This is due to the Arduino's inability to multi-thread. I am working on another program, using a [Particle Photon](https://store.particle.io/products/photon) for the driver and a server backend that will handle all commands, and solely send brightness values to the Photon.

Name     | Hex Value
:------- | :---------
`white`  | `0xFFFFFF`
`red`    | `0xFF0000`
`yellow` | `0xFFFF00`
`green`  | `0x00FF00`
`aqua`   | `0x00FFFF`
`blue`   | `0x0000FF`
`purple` | `0x800080`
`csh`    | `0xC1007C`

## Wiring

I based the wiring of the Arduino on a design on [Adafruit](https://adafruit.com). For the Red, Green and Blue wires, I used pins 3, 5 and 6 respectively on (these are all PWM pins).
![wiring diagram](https://cdn-learn.adafruit.com/assets/assets/000/002/692/original/led_strips_ledstripfet.gif?1448059609)
