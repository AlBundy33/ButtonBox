// Button Box
// usw with ProMicro

// enable two functions per button (short and long press)
#define ENABLE_DOUBLE_FUNCTION
// comment this line if you want to simulate a keyboard instead of a joystick
#define ENABLE_JOYSTICK

// library to handle the matrix
#include <Keypad.h>
// library to work with rotary encoders
#include <RotaryEncoder.h>
#ifdef ENABLE_JOYSTICK
#include <Joystick.h>
#else
#include <Keyboard.h>
// just some keys if we want to use the keyboard
char keys[62] = {
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};
#endif

// define the matrix pins
#define MATRIX_ROWS 3
#define MATRIX_COLUMNS 4
byte rowPins[MATRIX_ROWS] = {20, 19, 18};
byte colPins[MATRIX_COLUMNS] = {15, 14, 16, 10};

// fill matrix with values
byte matrix[MATRIX_ROWS][MATRIX_COLUMNS] = {
  { 0, 1, 2, 3},
  { 4, 5, 6, 7},
  { 8, 9,10,11},
};

// define the number of used buttons in our matrix
#define PHYSICAL_BUTTONS 12

// now define the rotaries (values are the used pins to read the data)
#define ROTARIES 4
RotaryEncoder rotaries[ROTARIES] = {
  {1,0},
  {2,3},
  {4,5},
  {6,7}
};

#ifdef ENABLE_DOUBLE_FUNCTION
#define VIRTUAL_BUTTONS PHYSICAL_BUTTONS * 2
#else
#define VIRTUAL_BUTTONS PHYSICAL_BUTTONS
#endif

struct buttonstate
{
  bool hold;
};
buttonstate buttonstates[VIRTUAL_BUTTONS];

Keypad button_box = Keypad(makeKeymap(matrix), rowPins, colPins, sizeof(rowPins), sizeof(colPins));

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
  button_box.setHoldTime(750);
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

void CheckAllButtons(void) {
  if (button_box.getKeys())
  {
    for (int i = 0; i < LIST_MAX; i++)
    {
      if ( button_box.key[i].stateChanged )
      {
        switch (button_box.key[i].kstate)
        {
          #ifdef ENABLE_DOUBLE_FUNCTION
            case PRESSED:
              break;
            case HOLD:
              buttonstates[button_box.key[i].kcode].hold = true;
              pressButton(button_box.key[i].kcode + PHYSICAL_BUTTONS); // long press
              break;
            case RELEASED:
                if (!buttonstates[button_box.key[i].kcode].hold)
                  pressButton(button_box.key[i].kcode); // short press
              break;
            case IDLE:
              buttonstates[button_box.key[i].kcode].hold = false;
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
  }
}


void CheckAllEncoders(void)
{
  for (int i = 0; i < 4; i++)
  {
    rotaries[i].tick();
    RotaryEncoder::Direction d = rotaries[i].getDirection();
    if (d == RotaryEncoder::Direction::COUNTERCLOCKWISE)
    {
      pressButton(VIRTUAL_BUTTONS + (i*2));
    }
    else if (d == RotaryEncoder::Direction::CLOCKWISE)
    {
      pressButton(VIRTUAL_BUTTONS + (i*2) + 1);
    };
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
    Keyboard.press(keys[idx]);
  else
    Keyboard.release(keys[idx]);
  #endif
}
