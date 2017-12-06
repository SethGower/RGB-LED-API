# RGB-LED-API

This is an Arduino sketch to drive an RGB LED strip in response to HTTP calls.

### Current state

Can currently only handle `POST` requests that are sent via `curl -X POST {location}/{hexCode}`.

### Future expansion

- Will handle more HTTP requests.
  - `GET` requests to return the current Color
  - `POST` requests to change the color.
- More functionality, such as fading effects.
- Google assistant support. 
