#include "hybrid/hybrid.h"
#include "vusb/usbdrv.h"

// Enabled or disabled by preprocessor to suit target OS
extern void conditional_homing();

void HybridDevice::begin(uint16_t width, uint16_t height) {
  set_dimensions(width, height);
  begin();
}

// Begin the USB connection properly
void HybridDevice::begin() {

    PORTD = 0; // TODO: Only for USB pins?
    DDRD |= ~USBMASK;

    // Reconnect so device is detected
    cli();
    usbDeviceDisconnect();
    delay_ms(250);
    usbDeviceConnect();

    usbInit();
        
    sei();

    // Wait two seconds for enumeration to finish. (Mac OS observed to take > 1000ms)
    for(int i=0; i<2000; i+=tx_delay) {
        usbPoll();
        delay_ms(tx_delay);
    }    
}

// Bytes 2 - 5 of report_stylus, xy coords
void HybridDevice::pos(uint16_t x, uint16_t y) {
report_stylus[2] = x & 0xFF;
report_stylus[3] = (x >> 8) & 0x7F;
report_stylus[4] = y & 0xFF;
report_stylus[5] = (y >> 8) & 0x7F;
}

// Send the report_stylus (Tell host what our imaginary stylus is doing)
void HybridDevice::update_stylus() {
    usbPoll();
    if (usbInterruptIsReady()) {
        usbSetInterrupt(report_stylus, sizeof(report_stylus));
    }
}

// Send the report_stylus (Tell host what our relative mouse is doing)
void HybridDevice::update_mouse() {
    usbPoll();
    if (usbInterruptIsReady()) {
        usbSetInterrupt(report_mouse, sizeof(report_mouse));
    }
}

// Encode the coordinate values for the HID report_stylus
int16_t HybridDevice::encode_val(int16_t val, uint16_t op_edge) {
  if (val < 0)  val = (op_edge + val) - 1;        // If negative, offset from opposite edge
  val = constrain((uint16_t)val, 0, op_edge);     // Clamp the value
  val = (((uint32_t)32767 * val) / op_edge);        // Scale to report_stylus val
  return val;
}

// Set the dimensions (outside of begin())
void HybridDevice::set_dimensions(uint16_t width, uint16_t height) {
  this->width = width;
  this->height = height;
}

// Move the cursor to (0,0), to force a position update in case user alters position between commands
// (OS specific option)
void HybridDevice::perform_homing() {
  pos(0, 0);
  set_in_range(true);
  update_stylus();
  delay_ms(tx_delay);
}

// Move the cursor (for display purposes)
void HybridDevice::position(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the stylus to the position
  conditional_homing();
  pos(x, y);
  set_in_range(true);
  update_stylus();
  delay_ms(tx_delay);

  //Pen up
  set_in_range(false);
  update_stylus();
  delay_ms(tx_delay);
}

// Click at the specified position
void HybridDevice::click(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move stylus
  conditional_homing();
  pos(x, y);
  set_in_range(true);
  update_stylus();
  delay_ms(tx_delay);

  //mouse button down
  set_left_button(true);
  update_mouse();

  //Pause for click to register
  delay_ms(tx_delay);

  //mouse button up
  set_left_button(false);
  update_mouse();
  delay_ms(tx_delay);

  //Remove the stylus
  set_in_range(false);
  update_stylus();
  delay_ms(tx_delay);
}

// Right click at the specified position
void HybridDevice::middle_click(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the stylus to the position
  conditional_homing();
  pos(x, y);
  set_in_range(true);
  update_stylus();
  delay_ms(tx_delay);

  //Press the mouse right button
  set_middle_button(true);
  update_mouse();

  //Pause for click to register
  delay_ms(tx_delay);

  //Release mouse right button
  set_middle_button(false);
  update_mouse();
  
  //Remove the stylus
  set_in_range(false);
  update_stylus();
  delay_ms(tx_delay);
}

// Right click at the specified position
void HybridDevice::right_click(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the stylus to the position
  conditional_homing();
  pos(x, y);
  set_in_range(true);
  update_stylus();
  delay_ms(tx_delay);

  //Press the mouse right button
  set_right_button(true);
  update_mouse();

  //Pause for click to register
  delay_ms(tx_delay);

  //Release mouse right button
  set_right_button(false);
  update_mouse();

  //Remove the stylus
  set_in_range(false);
  update_stylus();
  delay_ms(tx_delay);
}

// Double click at the specified position
void HybridDevice::double_click(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the stylus to the position
  conditional_homing();
  pos(x, y);
  update_stylus();
  delay_ms(tx_delay);

  //Button down
  set_left_button(true);
  update_mouse();

  //Pause for click to register
  delay_ms(tx_delay);

  //Button up
  set_left_button(false);
  update_mouse();

  //Pause between clicks
  delay_ms(100);

  //Button down, click number two
  set_left_button(true);
  update_mouse();

  //Pause for click to register
  delay_ms(tx_delay);

  //Button up
  set_left_button(false);
  update_mouse();

  //Remove the stylus
  set_in_range(false);
  update_stylus();
  delay_ms(tx_delay);
}

// Longpress at the specified position (mobile ui)
void HybridDevice::long_press(int16_t x, int16_t y, uint16_t duration) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the stylus to the position
  conditional_homing();
  pos(x, y);
  set_in_range(true);
  update_stylus();
  delay_ms(tx_delay);

  //Button down
  set_left_button(true);
  update_mouse();

  //Hold for the specified length
  for(uint16_t i = 0; i < duration; i += tx_delay)
    delay_ms(tx_delay);

  //Button up
  set_left_button(false);
  update_mouse();

  //Remove the stylus
  set_in_range(false);
  update_stylus();
  delay_ms(tx_delay);
}

// Perform a drag
void HybridDevice::drag(int16_t from_x, int16_t from_y, int16_t to_x, int16_t to_y, uint16_t duration) {
  //Check if duration is reasonable
  if(duration <= 100)  duration = 100;

  from_x = encode_val(from_x, width);
  from_y = encode_val(from_y, height);

  to_x = encode_val(to_x, width);
  to_y = encode_val(to_y, height);

  //Move the stylus to the start position, and contact the digitizer
  conditional_homing();
  pos(from_x, from_y);
  set_in_range(true);
  set_tip_switch(true);
  update_stylus();
  delay_ms(tx_delay);

  //mouse down
  set_left_button(true);
  update_mouse();
  delay_ms(tx_delay);

  //Move gradually towards end position
  uint16_t x = from_x;
  uint16_t y = from_y;
  
  uint32_t start = millis();
  uint32_t now;
  float progress = 0;
  do {
      now = millis();
      progress = (now - start) / (duration - 100.0);
      x = from_x + ((to_x - from_x) * progress);
      y = from_y + ((to_y - from_y) * progress);
      pos(x, y);
      update_stylus();
      delay_ms(tx_delay);
    } while (progress < 1);

  //Snap to end coords
  pos(to_x, to_y);
  update_stylus();
  delay_ms(tx_delay);

  //Release mouse
  set_left_button(false);
  update_mouse();
  delay_ms(tx_delay);

  //Remove stylus
  set_tip_switch(false);
  set_in_range(false);
  update_stylus();
  delay_ms(tx_delay);
}

// Press and hold indefinitely
void HybridDevice::hold(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the stylus to the position
  conditional_homing();
  pos(x, y);
  set_in_range(true);
  update_stylus();
  delay_ms(tx_delay);

  // Also press stylus to digitizer, apparently required by MacOS for drag operations
  set_tip_switch(true);
  update_stylus();
  delay_ms(tx_delay);

  set_left_button(true);
  update_mouse();
  delay_ms(tx_delay);
}

// Travel (while already holding)
// (no homing)
void HybridDevice::travel(int16_t from_x, int16_t from_y, int16_t to_x, int16_t to_y, uint16_t duration = 0) {
  //Check if duration is reasonable
  if(duration <= 100)  duration = 100;

  from_x = encode_val(from_x, width);
  from_y = encode_val(from_y, height);

  to_x = encode_val(to_x, width);
  to_y = encode_val(to_y, height);


  //Move the stylus to the start position
  pos(from_x, from_y);
  update_stylus();
  delay_ms(tx_delay);

  //Move gradually towards end position
  uint16_t x = from_x;
  uint16_t y = from_y;
  
  uint32_t start = millis();
  uint32_t now;
  float progress = 0;
  do {
      now = millis();
      progress = (now - start) / (duration - 100.0);
      x = from_x + ((to_x - from_x) * progress);
      y = from_y + ((to_y - from_y) * progress);
      pos(x, y);
      update_stylus();
      delay_ms(tx_delay);
    } while (progress < 1);

  //Snap to end coords
  pos(to_x, to_y);
  update_stylus();
  delay_ms(tx_delay);
}

// Release from HybridDevice::hold()
void HybridDevice::release() {
  //Release mouse
  set_left_button(false);
  update_mouse();
  delay_ms(tx_delay);

  //Pen up
  set_in_range(false);
  set_tip_switch(false);
  update_stylus();
  delay_ms(tx_delay);
}

// Scroll using the mouse wheel
void HybridDevice::scroll(int16_t at_x, int16_t at_y, int16_t amount) {
  at_x = encode_val(at_x, width);
  at_y = encode_val(at_y, height);

  //Move the cursor to the position
  pos(at_x, at_y);
  set_in_range(true);
  update_stylus();
  delay_ms(tx_delay);

  uint8_t sign = (amount > 0) ? -1 : 1; // Inverted, so that negative is up, positive down
  amount = abs(amount);

  //Scroll piece by piece until value is met
  while (amount > 0) {
    report_mouse[6] = min(127, amount);
    amount -= report_mouse[6];
    report_mouse[6] *= sign;
    update_mouse();
    delay_ms(tx_delay);
  }

  //Clear the scroll wheel, relative value needs to return to 0
  report_mouse[6] = 0;
  update_mouse();
  delay(tx_delay);

  //Remove the stylus
  set_in_range(false);
  update_stylus();
  delay_ms(tx_delay);
}