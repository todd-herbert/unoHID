# unoHID - API

- [Include Library](#include-library)
- [Methods](#methods)
  - [`Mouse.begin()`](#mousebegin)
  - [`Mouse.click()`](#mouseclick)
  - [`Mouse.end()`](#mouseend)
  - [`Mouse.move()`](#mousemove)
  - [`Mouse.press()`](#mousepress)
  - [`Mouse.release()`](#mouserelease)
  - [`Mouse.isPressed()`](#mouseispressed)
  - [`Mouse.doubleClick()`](#mousedoubleclick)
  - [`Mouse.longClick()`](#mouselongclick)
  - [`Mouse.scroll()`](#mousescroll)
  - [`Mouse.setTxDelay()`](#mousesettxdelay)
  - [`Keyboard.begin()`](#keyboardbegin)
  - [`Keyboard.end()`](#keyboardend)
  - [`Keyboard.press()`](#keyboardpress)
  - [`Keyboard.print()`](#keyboardprint)
  - [`Keyboard.println()`](#keyboardprintln)
  - [`Keyboard.release()`](#keyboardrelease)
  - [`Keyboard.releaseAll()`](#keyboardreleaseall)
  - [`Keyboard.write()`](#keyboardwrite)
  - [`Keyboard.setTxDelay()`](#keyboardsettxdelay)
- [Constants](#constants)
  - [Mouse Buttons](#mouse-buttons)
  - [Special Keys](#special-keys)
  - [Keyboard Layouts](#keyboard-layouts)


## Include Library
```cpp
#include <unoHID.h>
```

## Methods

### `Mouse.begin()`

Begins emulating the mouse connected to a computer. `begin()` must be called before controlling the computer. To end control, use `Mouse.end()`.

#### Syntax

```cpp
Mouse.begin()
```

#### Parameters

None

#### Example

```cpp
#include <unoHID.h>

void setup() {
  pinMode(8, INPUT);
}

void loop() {
  // begin mouse control when button is pressed
  if (digitalRead(8) == HIGH) {
    Mouse.begin();
  }
}
```

___
### `Mouse.click()`

Sends a momentary click to the computer at the location of the cursor. This is the same as pressing and immediately releasing the mouse button.

`Mouse.click()` defaults to the left mouse button.


#### Syntax
```cpp
Mouse.click()
Mouse.click(button)
```

#### Parameters

* _button_ (optional): which mouse button to press. Allowed data types: `MouseButton`.
    - `MOUSE_LEFT` (default)
    - `MOUSE_RIGHT`
    - `MOUSE_MIDDLE`

#### Example

```cpp
#include <unoHID.h>

void setup() {
  pinMode(8, INPUT);
  //begin mouse control
  Mouse.begin();
}

void loop() {
  //if the button is pressed, send a left mouse click
  if (digitalRead(8) == HIGH) {
    Mouse.click();
  }
}
```

___
### `Mouse.end()`

Stops emulating the mouse connected to a computer. To start control, use Mouse.begin().

#### Syntax

```cpp
Mouse.end()
```

#### Parameters

None

#### Example

```cpp
#include <unoHID.h>

void setup() {
  pinMode(8, INPUT);
  //begin mouse control
  Mouse.begin();
}

void loop() {
  //if the button is pressed, send a left mouse click
  //then end the Mouse emulation
  if (digitalRead(8) == HIGH) {
    Mouse.click();
    Mouse.end();
  }
}
```

___
### `Mouse.move()`

Moves the cursor on a connected computer. The motion onscreen is always relative to the cursor’s current location. Before using `Mouse.move()` you must call `Mouse.begin()`

#### Syntax

```cpp
Mouse.move(x, y)
Mouse.move(x, y, wheel)
```

#### Parameters

* _x_: amount to move along the x-axis. Positive values move cursor right. Allowed data types: `int`.
* _y_: amount to move along the y-axis. Positive values move cursor down. Allowed data types: `int`.
* _wheel_ (optional): amount to move scroll wheel. Positive values scroll mouse wheel up. Allowed data types: `signed char`.

#### Example

```cpp
#include <unoHI.h>

const int xAxis = A1;         //analog sensor for X axis
const int yAxis = A2;         // analog sensor for Y axis

int range = 12;               // output range of X or Y movement
int responseDelay = 2;        // response delay of the mouse, in ms
int threshold = range / 4;    // resting threshold
int center = range / 2;       // resting position value
int minima[] = {1023, 1023};  // actual analogRead minima for {x, y}
int maxima[] = {0, 0};        // actual analogRead maxima for {x, y}
int axis[] = {xAxis, yAxis};  // pin numbers for {x, y}
int mouseReading[2];          // final mouse readings for {x, y}


void setup() {
  Mouse.begin();
}

void loop() {
  // read and scale the two axes:
  int xReading = readAxis(0);
  int yReading = readAxis(1);

  // move the mouse:
  Mouse.move(xReading, yReading, 0);
  delay(responseDelay);
}

/*
  reads an axis (0 or 1 for x or y) and scales the
  analog input range to a range from 0 to <range>
*/

int readAxis(int axisNumber) {
  int distance = 0; // distance from center of the output range

  // read the analog input:
  int reading = analogRead(axis[axisNumber]);

  // of the current reading exceeds the max or min for this axis,
  // reset the max or min:
  if (reading < minima[axisNumber]) {
    minima[axisNumber] = reading;
  }
  if (reading > maxima[axisNumber]) {
    maxima[axisNumber] = reading;
  }

  // map the reading from the analog input range to the output range:
  reading = map(reading, minima[axisNumber], maxima[axisNumber], 0, range);

  // if the output reading is outside from the
  // rest position threshold,  use it:
  if (abs(reading - center) > threshold) {
    distance = (reading - center);
  }

  // the Y axis needs to be inverted in order to
  // map the movemment correctly:
  if (axisNumber == 1) {
    distance = -distance;
  }

  // return the distance for this axis:
  return distance;
}
```

___
### `Mouse.press()`

Sends a button press to a connected computer. A press is the equivalent of clicking and continuously holding the mouse button. A press is cancelled with `Mouse.release()`.

Before using `Mouse.press()`, you need to start communication with `Mouse.begin()`.

`Mouse.press()` defaults to a left button press.

#### Syntax

```cpp
Mouse.press()
Mouse.press(button)
```

#### Parameters

* _button_ (optional): which mouse button to press. Allowed data types: `MouseButton`.
    - `MOUSE_LEFT` (default)
    - `MOUSE_RIGHT`
    - `MOUSE_MIDDLE`

#### Example

```cpp
#include <unoHID.h>

void setup() {
  //The switch that will initiate the Mouse press
  pinMode(8, INPUT);
  //The switch that will terminate the Mouse press
  pinMode(9, INPUT);
  //begin mouse control
  Mouse.begin();
}

void loop() {
  //if the switch attached to pin 8 is closed, press and hold the left mouse button
  if (digitalRead(8) == HIGH) {
    Mouse.press();
  }
  //if the switch attached to pin 9 is closed, release the left mouse button
  if (digitalRead(9) == HIGH) {
    Mouse.release();
  }
}
```

___
### `Mouse.release()`

Sends a message that a previously pressed button (invoked through `Mouse.press()`) is released. `Mouse.release()` defaults to the left button.

#### Syntax

```cpp
Mouse.release()
Mouse.release(button)
```

#### Parameters

* _button_ (optional): which mouse button to press. Allowed data types: `MouseButton`.
    - `MOUSE_LEFT` (default)
    - `MOUSE_RIGHT`
    - `MOUSE_MIDDLE`

#### Example

```cpp
#include <unoHID.h>

void setup() {
  //The switch that will initiate the Mouse press
  pinMode(8, INPUT);
  //The switch that will terminate the Mouse press
  pinMode(9, INPUT);
  //begin mouse control
  Mouse.begin();
}

void loop() {
  //if the switch attached to pin 8 is closed, press and hold the left mouse button
  if (digitalRead(8) == HIGH) {
    Mouse.press();
  }
  //if the switch attached to pin 9 is closed, release the left mouse button
  if (digitalRead(9) == HIGH) {
    Mouse.release();
  }
}
```

___
### `Mouse.isPressed()`
Checks the current status of a mouse button, and reports if pressed or not.

#### Syntax

```cpp
Mouse.isPressed();
Mouse.isPressed(button);
```

#### Parameters

* _button_ (optional): which mouse button to press. Allowed data types: `MouseButton`.
    - `MOUSE_LEFT` (default)
    - `MOUSE_RIGHT`
    - `MOUSE_MIDDLE`

#### Returns

Reports whether a button is pressed or not. Data type: `bool`.

#### Example

```cpp
#include <Mouse.h>

void setup() {
  //The switch that will initiate the Mouse press
  pinMode(8, INPUT);
  //The switch that will terminate the Mouse press
  pinMode(9, INPUT);
  //Start serial communication with the computer
  Serial.begin(9600);
  //begin mouse control
  Mouse.begin();
}

void loop() {
  //a variable for checking the button's state
  int mouseState = 0;
  //if the switch attached to pin 8 is closed, press and hold the left mouse button and save the state ina  variable
  if (digitalRead(8) == HIGH) {
    Mouse.press();
    mouseState = Mouse.isPressed();
  }
  //if the switch attached to pin 9 is closed, release the left mouse button and save the state in a variable
  if (digitalRead(9) == HIGH) {
    Mouse.release();
    mouseState = Mouse.isPressed();
  }
  //print out the current mouse button state
  Serial.println(mouseState);
  delay(10);
}
```

___
### `Mouse.doubleClick()`

Sends a double click to the computer at the location of the cursor. This is the same as pressing and immediately releasing the mouse button, twice.

`Mouse.doubleClick()` defaults to the left mouse button.


#### Syntax
```cpp
Mouse.doubleClick()
Mouse.doubleClick(button)
```

#### Parameters

* _button_ (optional): which mouse button to use. Allowed data types: `MouseButton`.
    - `MOUSE_LEFT` (default)
    - `MOUSE_RIGHT`
    - `MOUSE_MIDDLE`

#### Example

```cpp
#include <unoHID.h>

void setup() {
  pinMode(8, INPUT);
  //begin mouse control
  Mouse.begin();
}

void loop() {
  //if the button is pressed, send a left mouse double click
  if (digitalRead(8) == HIGH) {
    Mouse.doubleClick();
  }
}
```

___
### `Mouse.longClick()`

At the location of the cursor, press and hold a mouse button for specified duration.

`Mouse.longClick()` defaults to the left mouse button.


#### Syntax
```cpp
Mouse.longClick(duration)
Mouse.doubleClick(duration, button)
```

#### Parameters

* _duration_: how long to hold the button, in milliseconds. Allowed data types: `unsigned int`.
* _button_ (optional): which mouse button to use. Allowed data types: `MouseButton`.
    - `MOUSE_LEFT` (default)
    - `MOUSE_RIGHT`
    - `MOUSE_MIDDLE`

#### Example

```cpp
#include <unoHID.h>

void setup() {
  pinMode(8, INPUT);
  //begin mouse control
  Mouse.begin();
}

void loop() {
  //if the button is pressed, click and hold for 1000ms, with left mouse button
  if (digitalRead(8) == HIGH) {
    Mouse.longClick(1000);
  }
}
```

___
### `Mouse.scroll()`

Scroll a long distance, using mouse wheel.

#### Syntax

```cpp
Mouse.scroll(amount)
```

#### Parameters

* amount: How far to scrol. Positive values scroll up. An `amount` of 1000 represents `Mouse.move(0,0,100)` x 10. Allowed data types: `int`.

#### Example

```cpp
#include <unoHID.h>

void setup() {
  pinMode(8, INPUT);
  //begin mouse control
  Mouse.begin();
}

void loop() {
  //when button pressed, scroll a large distance towards top of page
  if (digitalRead(8) == HIGH) {
    Mouse.scroll(1000);
  }
}
```

___
### `Mouse.setTxDelay()`

Specify a small delay to be added after every mouse command, for rate limiting.
An alternative to manually pausing between commands.

#### Syntax

```cpp
setTxDelay(duration)
```

#### Parameters

* _duration_: how long, in milliseconds, to pause after each mouse command. Default value is 0. Allowed data types: `unsigned int`.

#### Example

```cpp
#include <unoHID.h>

void setup() {
  pinMode(8, INPUT);
  Mouse.begin();            // Begin mouse control
  Mouse.setTxDelay(20);     // Pause for 20ms after each mouse command
};

void loop() {
  // Read potentiometer value
  int8_t vertical_movement = map( analogRead(A0), 0, 1023, -5, 5 );

  // Move mouse up or down based on reading
  Mouse.move(0, vertical_movement);

  // Normally, would need a pause here, or click will not register.
  // Pause is automatically added by setTxDelay

  // If button is pressed, left click at cursor position
  if (digitalRead(8) == HIGH) {
    Mouse.click();
  }
}
```

### `Keyboard.begin()`

`Keyboard.begin()` starts emulating a keyboard connected to a computer. To end control, use `Keyboard.end()`.

#### Syntax

```cpp
Keyboard.begin()
Keyboard.begin(layout)
```

#### Parameters

* _layout_: the keyboard layout to use. This parameter is optional and defaults to `KeyboardLayout_en_US`. 
    Currently, the library supports the following national keyboard layouts:
    - `KeyboardLayout_da_DK` - Denmark
    - `KeyboardLayout_de_DE` - Germany
    - `KeyboardLayout_en_US` - USA
    - `KeyboardLayout_fr_FR` - France
    - `KeyboardLayout_it_IT` - Italy
    - `KeyboardLayout_sv_SE` - Sweden

#### Example

```cpp
#include <unoHID.h>

void setup() {
  // make pin 8 an input and turn on the
  // pullup resistor so it goes high unless
  // connected to ground:
  pinMode(8, INPUT_PULLUP);
  Keyboard.begin();
}

void loop() {
  //if the button is pressed
  if (digitalRead(8) == LOW) {
    //Send the message
    Keyboard.print("Hello!");
  }
}
```

___
### `Keyboard.end()`

Stops the keyboard emulation to a connected computer. To start keyboard emulation, use Keyboard.begin().

#### Syntax

```cpp
Keyboard.end()
```

#### Parameters

None

#### Example

```cpp
#include <unoHID.h>

void setup() {
  //start keyboard communication
  Keyboard.begin();
  //send a keystroke
  Keyboard.print("Hello!");
  //end keyboard communication
  Keyboard.end();
}

void loop() {
  //do nothing
}
```

___
### `Keyboard.press()`

When called, `Keyboard.press()` functions as if a key were pressed and held on your keyboard. Useful when using modifier keys. To end the key press, use `Keyboard.release()` or `Keyboard.releaseAll()`.

#### Syntax

```cpp
Keyboard.press(key)
```

#### Parameters

* _key_: the key to press. Allowed data types: char

#### Returns

Number of key presses sent. Data type: size_t

#### Example

```cpp
#include <unoHID.h>

// use this option for OSX:
char ctrlKey = KEY_LEFT_GUI;
// use this option for Windows and Linux:
//  char ctrlKey = KEY_LEFT_CTRL;

void setup() {
  // make pin 8 an input and turn on the
  // pullup resistor so it goes high unless
  // connected to ground:
  pinMode(8, INPUT_PULLUP);
  // initialize control over the keyboard:
  Keyboard.begin();
}

void loop() {
  while (digitalRead(8) == HIGH) {
    // do nothing until pin 8 goes low
    delay(500);
  }
  delay(1000);
  // new document:
  Keyboard.press(ctrlKey);
  Keyboard.press('n');
  delay(100);
  Keyboard.releaseAll();
  // wait for new window to open:
  delay(1000);
}
```

___
### `Keyboard.print()`

Sends one or more keystrokes to a connected computer.

`Keyboard.print()` must be called after initiating `Keyboard.begin()`.


#### Syntax

```cpp
Keyboard.print(character)
Keyboard.println(characters)
```

#### Parameters

* _character_: a char or int to be sent to the computer as a keystroke.
* _characters_: a string to be sent to the computer as keystrokes.

#### Returns

Number of keystrokes sent. Data type: size_t.

#### Example

```cpp
#include <unoHID.h>

void setup() {
  // make pin 8 an input and turn on the
  // pullup resistor so it goes high unless
  // connected to ground:
  pinMode(8, INPUT_PULLUP);
  Keyboard.begin();
}

void loop() {
  //if the button is pressed
  if (digitalRead(8) == LOW) {
    //Send the message
    Keyboard.print("Hello!");
  }
}
```

___
### `Keyboard.println()`

Sends one or more keystrokes to a connected computer, followed by a keystroke on the Enter key.

`Keyboard.println()` must be called after initiating `Keyboard.begin()`.


#### Syntax

```cpp
Keyboard.println()
Keyboard.println(character)
Keyboard.println(characters)
```

#### Parameters

* _character_: a char or int to be sent to the computer as a keystroke, followed by Enter.
* _characters_: a string to be sent to the computer as keystrokes, followed by Enter.

#### Returns

Number of keystrokes sent. Data type: size_t.

#### Example

```cpp
#include <unoHID.h>

void setup() {
  // make pin 8 an input and turn on the
  // pullup resistor so it goes high unless
  // connected to ground:
  pinMode(8, INPUT_PULLUP);
  Keyboard.begin();
}

void loop() {
  //if the button is pressed
  if (digitalRead(8) == LOW) {
    //Send the message
    Keyboard.println("Hello!");
  }
}
```

___
### `Keyboard.release()`

Lets go of the specified key. See `Keyboard.press()` for more information.

#### Syntax

```cpp
Keyboard.release(key)
```

#### Parameters

* _key_: the key to release. Allowed data types: `char`.

#### Returns

The number of keeys released. Data type: size_t.

#### Example

```cpp
#include <unoHID.h>

// use this option for OSX:
char ctrlKey = KEY_LEFT_GUI;
// use this option for Windows and Linux:
//  char ctrlKey = KEY_LEFT_CTRL;

void setup() {
  // make pin 8 an input and turn on the
  // pullup resistor so it goes high unless
  // connected to ground:
  pinMode(8, INPUT_PULLUP);
  // initialize control over the keyboard:
  Keyboard.begin();
}

void loop() {
  while (digitalRead(8) == HIGH) {
    // do nothing until pin 8 goes low
    delay(500);
  }
  delay(1000);
  // new document:
  Keyboard.press(ctrlKey);
  Keyboard.press('n');
  delay(100);
  Keyboard.release(ctrlKey);
  Keyboard.release('n');
  // wait for new window to open:
  delay(1000);
}
```

___
### `Keyboard.releaseAll()`

Lets go of all keys currently pressed. See `Keyboard.press()` for additional information.

#### Syntax

```cpp
Keyboard.releaseAll()
```

#### Parameters

None

#### Example

```cpp
#include <unoHID.h>

// use this option for OSX:
char ctrlKey = KEY_LEFT_GUI;
// use this option for Windows and Linux:
//  char ctrlKey = KEY_LEFT_CTRL;

void setup() {
  // make pin 8 an input and turn on the
  // pullup resistor so it goes high unless
  // connected to ground:
  pinMode(8, INPUT_PULLUP);
  // initialize control over the keyboard:
  Keyboard.begin();
}

void loop() {
  while (digitalRead(8) == HIGH) {
    // do nothing until pin 8 goes low
    delay(500);
  }
  delay(1000);
  // new document:
  Keyboard.press(ctrlKey);
  Keyboard.press('n');
  delay(100);
  Keyboard.releaseAll();
  // wait for new window to open:
  delay(1000);
}
```

___
### `Keyboard.write()`

Sends a keystroke to a connected computer. This is similar to pressing and releasing a key on your keyboard. You can send some ASCII characters or the additional keyboard modifiers and special keys.

Only ASCII characters that are on the keyboard are supported. For example, ASCII 8 (backspace) would work, but ASCII 25 (Substitution) would not. When sending capital letters, Keyboard.write() sends a shift command plus the desired character, just as if typing on a keyboard. If sending a numeric type, it sends it as an ASCII character (ex. Keyboard.write(97) will send 'a').

For a complete list of ASCII characters, see [](ASCIITable.com).

#### Syntax

```cpp
Keyboard.write(character)
```

#### Parameters

* _character_: a char or int to be sent to the computer. Can be sent in any notation that’s acceptable for a char. For example, all of the below are acceptable and send the same value, 65 or ASCII A:

```cpp
Keyboard.write(65);         // sends ASCII value 65, or A
Keyboard.write('A');        // same thing as a quoted character
Keyboard.write(0x41);       // same thing in hexadecimal
Keyboard.write(0b01000001); // same thing in binary (weird choice, but it works)
```

#### Example

```cpp
#include <unoHID.h>

void setup() {
  // make pin 8 an input and turn on the
  // pullup resistor so it goes high unless
  // connected to ground:
  pinMode(8, INPUT_PULLUP);
  Keyboard.begin();
}

void loop() {
  //if the button is pressed
  if (digitalRead(8) == LOW) {
    //Send an ASCII 'A',
    Keyboard.write(65);
  }
}
```

___
### `Keyboard.setTxDelay()`

Specify a small delay to be added after every keyboard command, for rate limiting.
An alternative to manually pausing between commands. Too low of a value can cause issues with some hardware.

#### Syntax

```cpp
setTxDelay(duration)
```

#### Parameters

* _duration_: how long, in milliseconds, to pause after each keyboard command. Default value is 0. Allowed data types: `unsigned int`.


## Constants

### Mouse Buttons

(`enum MouseButton uint8_t` in `mouse.h`)

 Identifier      | Value (`uint8_t`)
----------------|---------------------
 `MOUSE_LEFT`   | 1
 `MOUSE_RIGHT`  | 2
 `MOUSE_MIDDLE` | 3

### Special Keys

(Defined in Keyboard.h)

 Identifier         | Value (macro) | Comments
 -------------------|---------------| -----------------------------------
 `KEY_LEFT_CTRL`    | 0x80          | 
 `KEY_LEFT_SHIFT`   | 0x81          |
 `KEY_LEFT_ALT`     | 0x82          |
 `KEY_LEFT_GUI`     | 0x83          | Windows Key / Apple Key / Command Key
 `KEY_RIGHT_CTRL`   | 0x84          |
 `KEY_RIGHT_SHIFT`  | 0x85          |
 `KEY_RIGHT_ALT`    | 0x86          |
 `KEY_RIGHT_GUI`    | 0x87          |
 `KEY_UP_ARROW`     | 0xDA          |
 `KEY_DOWN_ARROW`   | 0xD9          |
 `KEY_LEFT_ARROW`   | 0xD8          |
 `KEY_RIGHT_ARROW`  | 0xD7          |
 `KEY_BACKSPACE`    | 0xB2          |
 `KEY_TAB`          | 0xB3          |
 `KEY_RETURN`       | 0xB0          | Enter
 `KEY_ESC`          | 0xED          |
 `KEY_MENU`         | 0xB1          | "Keyboard Application" in USB standard
 `KEY_INSERT`       | 0xD1          |
 `KEY_DELETE`       | 0xD4          |
 `KEY_PAGE_UP`      | 0xD3          |
 `KEY_PAGE_DOWN`    | 0xD6          |
 `KEY_HOME`         | 0xD2          |
 `KEY_END`          | 0xD5          |
 `KEY_CAPS_LOCK`    | 0xC1          |
 `KEY_PRINT_SCREEN` | 0xCE          | Print Screen / SysRq
 `KEY_SCROLL_LOCK`  | 0xCF          |
 `KEY_PAUSE`        | 0xD0          | Pause / Break
 `KEY_NUM_LOCK`     | 0xDB          |
 `KEY_KP_SLASH`     | 0xDC          |
 `KEY_KP_ASTERISK`  | 0xDD          |
 `KEY_KP_MINUS`     | 0xDE          |
 `KEY_KP_PLUS`      | 0xDF          |
 `KEY_KP_ENTER`     | 0xE0          |
 `KEY_KP_1`         | 0xE1          |
 `KEY_KP_2`         | 0xE2          |
 `KEY_KP_3`         | 0xE3          |
 `KEY_KP_4`         | 0xE4          |
 `KEY_KP_5`         | 0xE5          |
 `KEY_KP_6`         | 0xE6          |
 `KEY_KP_7`         | 0xE7          |
 `KEY_KP_8`         | 0xE8          |
 `KEY_KP_9`         | 0xE9          |
 `KEY_KP_0`         | 0xEA          |
 `KEY_KP_DOT`       | 0xEB          |
 `KEY_F1`           | 0xC2          |
 `KEY_F2`           | 0xC3          |
 `KEY_F3`           | 0xC4          |
 `KEY_F4`           | 0xC5          |
 `KEY_F5`           | 0xC6          |
 `KEY_F6`           | 0xC7          |
 `KEY_F7`           | 0xC8          |
 `KEY_F8`           | 0xC9          |
 `KEY_F9`           | 0xCA          |
 `KEY_F10`          | 0xCB          |
 `KEY_F11`          | 0xCC          |
 `KEY_F12`          | 0xCD          |
 `KEY_F13`          | 0xF0          |
 `KEY_F14`          | 0xF1          |
 `KEY_F15`          | 0xF2          |
 `KEY_F16`          | 0xF3          |
 `KEY_F17`          | 0xF4          |
 `KEY_F18`          | 0xF5          |
 `KEY_F19`          | 0xF6          |
 `KEY_F20`          | 0xF7          |
 `KEY_F21`          | 0xF8          |
 `KEY_F22`          | 0xF9          |
 `KEY_F23`          | 0xFA          |
 `KEY_F24`          | 0xFB          |

#### Regional

 Identifier                 | Value | File
 ---------------------------|-------|:------------------------------:
 KEY_A_RING                 | Å     | `#include "Keyboard_da_DK.h"`
 KEY_SLASHED_O              | Ø     | “
 KEY_ASH                    | Æ     | “
 KEY_UMLAUT                 | ¨     | “
 KEY_ACUTE_ACC              | ´     | “
 KEY_CIRCUMFLEX             | ^     | `#include "Keyboard_de_DE.h"`  
 KEY_ESZETT                 | ß     | “
 KEY_ACUTE                  | ´     | “
 KEY_U_UMLAUT               | Ü     | “
 KEY_O_UMLAUT               | Ö     | “
 KEY_A_UMLAUT               | Ä     | “
 KEY_MASCULIE_ORDINAL       | º     | `#include "Keyboard_es_ES.h"`
 KEY_INVERTED_EXCLAMATION   | ¡     | “
 KEY_GRAVE                  | `     | “
 KEY_N_TILDE                | Ñ     | “
 KEY_ACUTE                  | ´     | “
 KEY_C_CEDILLA              | Ç     | “
 KEY_SUPERSCRIPT_TWO        | ²     | `#include "Keyboard_fr_FR.h"`     
 KEY_E_ACUTE                | É     | “
 KEY_E_GRAVE                | È     | “
 KEY_C_CEDILLA              | Ç     | “
 KEY_A_GRAVE                | À     | “
 KEY_CIRCUMFLEX             | ^     | “
 KEY_U_GRAVE                | Ù     | “
 KEY_I_GRAVE                | Ì     | `#include "Keyboard_it_IT.h"`    
 KEY_E_GRAVE                | È     | “
 KEY_O_GRAVE                | Ò     | “
 KEY_A_GRAVE                | À     | “
 KEY_U_GRAVE                | Ù     | “
 KEY_A_RING                 | Å     | `#include "Keyboard_sv_SV.h"`     
 KEY_A_UMLAUT               | Ä     | “
 KEY_O_UMLAUT               | Ö     | “
 KEY_UMLAUT                 | ¨     | “
 KEY_ACCUTE_ACC             | ´     | “

### Keyboard Layouts

 Identifier             | Locale
------------------------|---------
 `KeyboardLayout_da_DK` | Denmark
 `KeyboardLayout_de_DE` | Germany
 `KeyboardLayout_en_US` | USA
 `KeyboardLayout_fr_FR` | France
 `KeyboardLayout_it_IT` | Italy
 `KeyboardLayout_sv_SE` | Sweden
