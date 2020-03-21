// Button Box
// usw with ProMicro

// enable two functions per button (short and long press)
#define ENABLE_DOUBLE_FUNCTION
// comment this line if you want to simulate a keyboard instead of a joystick
#define ENABLE_JOYSTICK

// Keypad by Alexander Brevig, Mark Stanley
// https://playground.arduino.cc/Code/Keypad/
#include <Keypad.h>
// RotaryEncoder by Matthias Hertel
// https://github.com/mathertel/RotaryEncoder
#include <RotaryEncoder.h>
#ifdef ENABLE_JOYSTICK
// https://github.com/MHeironimus/ArduinoJoystickLibrary
#include <Joystick.h>
#else
#include <Keyboard.h>
#endif

// define the matrix rows and colums
#define MATRIX_ROWS 3
#define MATRIX_COLUMNS 4
// define the matrix pins
byte rowPins[MATRIX_ROWS] = {20, 19, 18};
byte colPins[MATRIX_COLUMNS] = {15, 14, 16, 10};

// fill matrix with values
// TODO: this is only needed to initialize the keypad and should be created by a function...
byte matrix[MATRIX_ROWS][MATRIX_COLUMNS] = {
  { 0, 1, 2, 3},
  { 4, 5, 6, 7},
  { 8, 9,10,11},
};

// define the number of used buttons in our matrix (by default all buttons are used)
#define PHYSICAL_BUTTONS MATRIX_ROWS * MATRIX_COLUMNS

// now define the rotaries (values are the used pins to read the data)
#define ROTARIES 4
RotaryEncoder rotaries[ROTARIES] = {
  {1,0}, // I had to switch the pins because I solderd them wrong :-)
  {2,3},
  {4,5},
  {6,7}
};

#ifdef ENABLE_DOUBLE_FUNCTION
#define VIRTUAL_BUTTONS PHYSICAL_BUTTONS * 2
#else
#define VIRTUAL_BUTTONS PHYSICAL_BUTTONS
#endif

// the keys to send if we want to use the keyboard
// count should be twice the count of your physical buttons plus two times (left and right direction) of your rotary count
// first part is for short presses and the second one for long presses
char KEYBOARD_KEYS[VIRTUAL_BUTTONS + (ROTARIES * 2)] = {
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
  '0', '1', '2', '3', '4', '5', '6', '7'
};

bool button_hold_states[VIRTUAL_BUTTONS];

Keypad button_box = Keypad(makeKeymap(matrix), rowPins, colPins, MATRIX_ROWS, MATRIX_COLUMNS);

#ifdef ENABLE_JOYSTICK
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK, VIRTUAL_BUTTONS + (ROTARIES * 2), 0,
                   false, false, false, false, false, false,
                   false, false, false, false, false);
#endif

void setup()
{
  button_box.setDebounceTime(100);
  #ifdef ENABLE_DOUBLE_FUNCTION
  button_box.setHoldTime(500);
  #endif
  #ifdef ENABLE_JOYSTICK
  Joystick.begin();
  #endif
}

void loop()
{
  CheckAllEncoders();
  CheckAllButtons();
}

void CheckAllButtons() {
  if (!button_box.getKeys())
    return;

  for (int i = 0; i < LIST_MAX; i++)
  {
    if ( !button_box.key[i].stateChanged )
      continue;

    switch (button_box.key[i].kstate)
    {
      #ifdef ENABLE_DOUBLE_FUNCTION
        case PRESSED:
          break;
        case HOLD:
          button_hold_states[button_box.key[i].kcode] = true;
          pressButton(button_box.key[i].kcode + PHYSICAL_BUTTONS, true); // long press
          break;
        case RELEASED:
            if (button_hold_states[button_box.key[i].kcode])
              pressButton(button_box.key[i].kcode + PHYSICAL_BUTTONS, false); // end long press  
            else
              pressButton(button_box.key[i].kcode); // short press
          break;
        case IDLE:
          button_hold_states[button_box.key[i].kcode] = false;
          break;
      #else
        case PRESSED:
        case HOLD:
          pressButton(button_box.key[i].kchar, true);
          break;
        case RELEASED:
        case IDLE:
          pressButton(button_box.key[i].kchar, false);
          break;        
      #endif
    }
  }
}

void CheckAllEncoders()
{
  for (int i = 0; i < ROTARIES; i++)
  {
    rotaries[i].tick();
    RotaryEncoder::Direction d = rotaries[i].getDirection();
    if (d == RotaryEncoder::Direction::COUNTERCLOCKWISE)
      pressButton(VIRTUAL_BUTTONS + (i*2));
    else if (d == RotaryEncoder::Direction::CLOCKWISE)
      pressButton(VIRTUAL_BUTTONS + (i*2) + 1);
  }
}

void pressButton(int idx)
{
  pressButton(idx, true);
  delay(50);
  pressButton(idx, false);
}

void pressButton(int idx, bool down)
{
  #ifdef ENABLE_JOYSTICK
  Joystick.setButton(idx, down ? 1 : 0);
  #else
  if (down)
    Keyboard.press(KEYBOARD_KEYS[idx]);
  else
    Keyboard.release(KEYBOARD_KEYS[idx]);
  #endif
}
