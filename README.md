Source code for my button box with an Arduino Pro Micro

The button box constis of a button matrix with 3 rows and 4 buttons per row.

the first row contains 4 rotary encoders.

so button numbers are
```
 1,  2,  3,  4
 5,  6,  7,  8
 9, 10, 11, 12
```
followed by
```
13, 14, 15, 16
17, 18, 19, 20
```
where these buttons are
- rotary 1 left, right
- rotary 2 left, right
- rotary 3 left, right
- rotary 4 left, right

original code: https://github.com/AM-STUDIO/32-FUNCTION-BUTTON-BOX

Fo build instructions see also amstudios video: https://www.youtube.com/watch?v=Z7Sc4MJ8RPM

You have to install this library in Arduino IDE: https://github.com/MHeironimus/ArduinoJoystickLibrary
