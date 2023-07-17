# Function list

`<>` indicate optional arguments, when `EMULATE_MOUSE`

## begin
#### `begin( <width>, <height> )`
Begin USB communications. Passing `<width>` and `<height>` sets the dimensions of the screen, in pixels, in mm, or any other system you find convenient.

## set_dimensions
#### `set_dimensions( width, height )`
Sets the dimensions of the screen, in pixels, in mm, or any other system you find convenient.


## click
#### `click( <x>, <y> )`
Perform a single click at (x, y), or wherever the cursor is currently located.

## right_click
#### `right_click( <x>, <y> )`
Peform a right click at (x, y), or wherever the cursor is currently located.

## double_click
#### `double_click( <x>, <y> )`
Peform a double click at (x, y), or wherever the cursor is currently located.

## middle_click
#### `middle_click( <x>, <y> )`
**Only with `EMULATE_MOUSE` or `EMULATE_HYBRID`**
Press the middle button (scroll wheel) at point (x,y), or wherever the cursor is currently located.

## long_press
#### `long_press( <x>, <y>, duration )`
Press, at point (x, y), or wherever the cursor is currently located, for "duration" in milliseconds.

## scroll
#### `scroll( <x>, <y>, amount )`
Place the cursor at point (x,y) if specified, then scroll by amount using the wheel. Postive values scroll down, negative values scroll up. The distance scrolled is consistent, but somewhat arbitrary. You will need to manually determine the correct value.


## drag
#### `drag ( from_x, from_y, to_x, to_y, duration )`
Perform a click and drag maneuver, with duration in ms. Minimum duration 100ms

## hold
#### `hold( <x>, <y> )`
Press and hold indefinitely at (x, y), or wherever the cursor is currently located.

## travel
#### `travel ( from_x, from_y, to_x, to_y, duration )`
Gradually move the cursor from point to point, with duration in ms. Minimum duration 100ms. Useful with `hold` and `release`.

## release
#### `release ()`
Stop an indefinite press started by `hold`.

## position
#### `position( x, y )`
Move the cursor to point (x,y). This low level command is not required for most operation, but may be helpful in certain situations.

## blink
#### `blink ( x, y )`
**Only with `EMULATE_STYLUS`**
Force the cursor to briefly appear and dissapear. As `EMULATE_STYLUS` tends not to display a visible cursor, this function is useful in the DevKit, for figuring out co-ordinate values.