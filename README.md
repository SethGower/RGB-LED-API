# RGB-LED-API

This is an Arduino sketch to drive an RGB LED strip in response to HTTP calls.

## Current state

Can currently only handle `POST` requests that are sent via `curl -X POST {location}/{color}`, the color can be in the form of a hexcode `0xFFFFFF` or one of the following colors, all case insensitive.

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

## Future expansion

- More functionality, such as fading effects.
- Google assistant support.
