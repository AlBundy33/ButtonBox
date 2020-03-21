Source code for my button box with an Arduino Pro Micro

The button box constis of a button matrix with 3 rows and 4 buttons per row.

The first row contains 4 rotary encoders wih push funcion.

Each push button simulates two buttons - one for short press and the other for long press.

so button numbers are
```
 1,  2,  3,  4
 5,  6,  7,  8
 9, 10, 11, 12
```
and some extra virtual buttons for long button presses
```
13, 14, 15, 16
17, 18, 19, 20
21, 22, 23, 24
```
followed by buttons for the rotaries
```
25, 26, 27, 28
29, 30, 31, 32
```
where these buttons are
- rotary 1 left, right
- rotary 2 left, right
- rotary 3 left, right
- rotary 4 left, right

original code: https://github.com/AM-STUDIO/32-FUNCTION-BUTTON-BOX

For build instructions see also amstudios video: https://www.youtube.com/watch?v=Z7Sc4MJ8RPM

You have to install this library in Arduino IDE: https://github.com/MHeironimus/ArduinoJoystickLibrary

To install Keypad.h see also https://forum.arduino.cc/index.php?topic=511429.0 

For PS4 and XBox compatibility it seems that using Keyboard.h is better than Joystick.h
https://forums.codemasters.com/topic/41266-f-yeahdiy-button-box-ps4-done-and-working/
