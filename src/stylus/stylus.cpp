#include "stylus/stylus.h"
#include "vusb/usbdrv.h"

// Enabled or disabled by preprocessor to suit target OS
extern void conditional_homing();

void StylusDevice::begin(uint16_t width, uint16_t height) {
  set_dimensions(width, height);
  begin();
}

// Begin the USB connection properly
void StylusDevice::begin() {

    PORTD = 0; // TODO: Only for USB pins?
    DDRD |= ~USBMASK;

    // Reconnect so device is detected
    cli();
    usbDeviceDisconnect();
    delay_ms(250);
    usbDeviceConnect();

    usbInit();
        
    sei();

    // Wait one second for enumeration to finish. (Android USB OTG requires >500ms)
    for(int i=0; i<2000; i++) {
        usbPoll();
        delay_ms(1);
    }    
}

// Bytes 2 - 5 of report_stylus, xy coords
void StylusDevice::pos(uint16_t x, uint16_t y) {
report_stylus[2] = x & 0xFF;
report_stylus[3] = (x >> 8) & 0x7F;
report_stylus[4] = y & 0xFF;
report_stylus[5] = (y >> 8) & 0x7F;
}

// Send the report_stylus (Tell host what our auxilliary mouse is doing)
void StylusDevice::update_mouse() {
    usbPoll();
    if (usbInterruptIsReady()) {
        usbSetInterrupt(report_mouse, sizeof(report_mouse));
    }
}

// Send the report_stylus (Tell host what our imaginary stylus is doing)
void StylusDevice::update_stylus() {
    usbPoll();
    if (usbInterruptIsReady()) {
        usbSetInterrupt(report_stylus, sizeof(report_stylus));
    }
}

// Encode the coordinate values for the HID report_stylus
int16_t StylusDevice::encode_val(int16_t val, uint16_t op_edge) {
  if (val < 0)  val = (op_edge + val) - 1;        // If negative, offset from opposite edge
  val = constrain((uint16_t)val, 0, op_edge);     // Clamp the value
  val = (((uint32_t)32767 * val) / op_edge);        // Scale to report_stylus val
  return val;
}

// Set the dimensions (outside of begin())
void StylusDevice::set_dimensions(uint16_t width, uint16_t height) {
  this->width = width;
  this->height = height;
}

// Move the cursor to (0,0), to force a position update in case user alters position between commands
// (OS specific option)

void StylusDevice::perform_homing() {
  pos(0,0);
  update_stylus();
  delay_ms(tx_delay);
}

// Display the cursor (for debugging)
void StylusDevice::blink(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the stylus to the position
  conditional_homing();
  pos(x, y);
  update_stylus();
  delay_ms(tx_delay);

  //Show and hide cursor
  for (uint8_t blink = 0; blink < 3; blink++) {
    //Pen down
    set_in_range(true);

    for(uint16_t i = 0; i < 400; i+=tx_delay) {
      update_stylus();
      delay_ms(tx_delay);
    }

    //Pen up
    set_in_range(false);

    for(uint16_t i = 0; i < 400; i+=tx_delay) {
      update_stylus();
      delay_ms(tx_delay);
    }
  }
}

// Click at the specified position
void StylusDevice::click(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the stylus to the position
  conditional_homing();
  pos(x, y);
  update_stylus();
  delay_ms(tx_delay);

  //Pen down
  set_in_range(true);
  set_tip_switch(true);
  update_stylus();

  //Pause for click to register
  delay_ms(tx_delay);

  //Pen up
  set_in_range(false);
  set_tip_switch(false);
  update_stylus();
}

// Right click at the specified position
void StylusDevice::right_click(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the stylus to the position
  conditional_homing();
  pos(x, y);
  update_stylus();
  delay_ms(tx_delay);

  //Press barrel switch button, and pen down
  set_in_range(true);
  set_tip_switch(true);
  set_barrel_switch(true);
  update_stylus();

  //Pause for click to register
  delay_ms(tx_delay);

  //Release stylus button, and pen up
  set_in_range(false);
  set_tip_switch(false);
  set_barrel_switch(false);
  update_stylus();
}

// Double click at the specified position
void StylusDevice::double_click(int16_t x, int16_t y) {
  //We could just call click() twice, but then it would recalculate all the values.
  //We'll do it the long way.

  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the stylus to the position
  conditional_homing();
  pos(x, y);
  update_stylus();
  delay_ms(tx_delay);

  //Pen down
  set_in_range(true);
  set_tip_switch(true);
  update_stylus();

  //Pause for click to register
  delay_ms(tx_delay);

  //Pen up
  set_in_range(false);
  set_tip_switch(false);
  update_stylus();

  //Pause between clicks
  delay_ms(100);

  //Pen down, click number two
  set_in_range(true);
  set_tip_switch(true);
  update_stylus();

  //Pause for click to register
  delay_ms(tx_delay);

  //Pen up
  set_in_range(false);
  set_tip_switch(false);
  update_stylus();
}

// Longpress at the specified position (mobile ui)
void StylusDevice::long_press(int16_t x, int16_t y, uint16_t duration) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the stylus to the position
  conditional_homing();
  pos(x, y);
  update_stylus();
  delay_ms(tx_delay);

  //Pen down
  set_in_range(true);
  set_tip_switch(true);
  update_stylus();

  //Hold for the specified length
  for(uint16_t i = 0; i < duration; i += tx_delay)
    delay_ms(tx_delay);

  //Pen up
  set_in_range(false);
  set_tip_switch(false);
  update_stylus();
}

// Perform a drag
void StylusDevice::drag(int16_t from_x, int16_t from_y, int16_t to_x, int16_t to_y, uint16_t duration) {
  //Check if duration is reasonable
  if(duration <= 100)  duration = 100;

  from_x = encode_val(from_x, width);
  from_y = encode_val(from_y, height);

  to_x = encode_val(to_x, width);
  to_y = encode_val(to_y, height);

  //Move the stylus to the start position
  conditional_homing();
  pos(from_x, from_y);
  update_stylus();
  delay_ms(tx_delay);

  //Pen down
  set_in_range(true);
  set_tip_switch(true);
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


  //Pen up
  set_in_range(false);
  set_tip_switch(false);
  update_stylus();
}

// Press and hold indefinitely
void StylusDevice::hold(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the stylus to the position
  conditional_homing();
  pos(x, y);
  update_stylus();
  delay_ms(tx_delay);

  //Pen down
  set_in_range(true);
  set_tip_switch(true);
  update_stylus();
}

// Travel (while already holding)
// (no homing)
void StylusDevice::travel(int16_t from_x, int16_t from_y, int16_t to_x, int16_t to_y, uint16_t duration) {
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

// Release from StylusDevice::hold()
void StylusDevice::release() {
  //Pen up
  set_in_range(false);
  set_tip_switch(false);
  update_stylus();
}

// Scroll using the secondary mouse device
void StylusDevice::scroll(int16_t at_x, int16_t at_y, int16_t amount) {
  at_x = encode_val(at_x, width);
  at_y = encode_val(at_y, height);

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

// Move the cursor (for display purposes)
void StylusDevice::position(int16_t x, int16_t y) {
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