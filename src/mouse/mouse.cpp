#include "mouse/mouse.h"
#include "vusb/usbdrv.h"

// Enabled or disabled by preprocessor to suit target OS
extern void conditional_homing();

void MouseDevice::begin(uint16_t width, uint16_t height) {
  set_dimensions(width, height);
  begin();
}

// Begin the USB connection properly
void MouseDevice::begin() {

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
    for(int i=0; i<1000; i++) {
        usbPoll();
        delay_ms(1);
    }    
}

// Bytes 2 - 5 of report, xy coords
void MouseDevice::pos(uint16_t x, uint16_t y) {
report[2] = x & 0xFF;
report[3] = (x >> 8) & 0x7F;
report[4] = y & 0xFF;
report[5] = (y >> 8) & 0x7F;
}

// Send the report (Tell host what our mouse is doing)
void MouseDevice::update() {
    usbPoll();
    if (usbInterruptIsReady()) {
        usbSetInterrupt(report, sizeof(report));
    }
}

// Encode the coordinate values for the HID report
int16_t MouseDevice::encode_val(int16_t val, uint16_t op_edge) {
  if (val < 0)  val = (op_edge + val) - 1;        // If negative, offset from opposite edge
  val = constrain((uint16_t)val, 0, op_edge);     // Clamp the value
  val = (((uint32_t)32767 * val) / op_edge);        // Scale to report val
  return val;
}

// Set the dimensions (outside of begin())
void MouseDevice::set_dimensions(uint16_t width, uint16_t height) {
  this->width = width;
  this->height = height;
}

// Move the cursor to (0,0), to force a position update in case user alters position between commands
// (OS specific option)
void MouseDevice::perform_homing() {
  pos(0,0);
  update();
  delay_ms(tx_delay);
}

// Move the cursor (for display purposes)
void MouseDevice::position(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the cursor to the position
  conditional_homing();
  pos(x, y);
  update();
  delay_ms(tx_delay);
}

// Click at the specified position
void MouseDevice::click() {
  //Button down
  set_left_button(true);
  update();

  //Pause for click to register
  delay_ms(tx_delay);

  //Button up
  set_left_button(false);
  update();
}
void MouseDevice::click(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the cursor to the position
  conditional_homing();
  pos(x, y);
  update();
  delay_ms(tx_delay);

  click();
}

// Click at the specified position
void MouseDevice::middle_click() {
  //Button down
  set_middle_button(true);
  update();

  //Pause for click to register
  delay_ms(tx_delay);

  //Button up
  set_middle_button(false);
  update();
}
void MouseDevice::middle_click(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the cursor to the position
  conditional_homing();
  pos(x, y);
  update();
  delay_ms(tx_delay);

  middle_click();
}

// Right click at the specified position
void MouseDevice::right_click() {
  //Right button down
  set_right_button(true);
  update();

  //Pause for click to register
  delay_ms(tx_delay);

  //Right button up
  set_right_button(false);
  update();
}
void MouseDevice::right_click(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the cursor to the position
  conditional_homing();
  pos(x, y);
  update();
  delay_ms(tx_delay);

  right_click();
}

// Double click at the specified position
void MouseDevice::double_click() {
  //We could just call click() twice, but then it would recalculate all the values.
  //We'll do it the long way.

  //Button down
  set_left_button(true);
  update();

  //Pause for click to register
  delay_ms(tx_delay);

  //Button up
  set_left_button(false);
  update();

  //Pause between clicks
  delay_ms(100);

  //Button down, click number two
  set_left_button(true);
  update();

  //Pause for click to register
  delay_ms(tx_delay);

  //Button up
  set_left_button(false);
  update();
}
void MouseDevice::double_click(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the cursor to the position
  conditional_homing();
  pos(x, y);
  update();
  delay_ms(tx_delay);

  double_click();
}

// Longpress at the specified position (mobile ui)
void MouseDevice::long_press(uint16_t duration) {
  //Button down
  set_left_button(true);
  update();

  //Hold for the specified length
  for(uint16_t i = 0; i < duration; i += tx_delay)
    delay_ms(tx_delay);

  //Button up
  set_left_button(false);
  update();
}
void MouseDevice::long_press(int16_t x, int16_t y, uint16_t duration) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the cursor to the position
  conditional_homing();
  pos(x, y);
  update();
  delay_ms(tx_delay);

  long_press(duration);
}

// Perform a drag
void MouseDevice::drag(int16_t from_x, int16_t from_y, int16_t to_x, int16_t to_y, uint16_t duration) {
  //Check if duration is reasonable
  if(duration <= 100)  duration = 100;

  from_x = encode_val(from_x, width);
  from_y = encode_val(from_y, height);

  to_x = encode_val(to_x, width);
  to_y = encode_val(to_y, height);

  //Move the cursor to the start position
  conditional_homing();
  pos(from_x, from_y);
  update();
  delay_ms(tx_delay);

  //Button down
  set_left_button(true);
  update();
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
      update();
      delay_ms(tx_delay);
    } while (progress < 1);

  //Snap to end coords
  pos(to_x, to_y);
  update();
  delay_ms(tx_delay);


  //Button up
  set_left_button(false);
  update();
}

// Press and hold indefinitely
void MouseDevice::hold() {
  //Pen down
  set_left_button(true);
  update();
}
void MouseDevice::hold(int16_t x, int16_t y) {
  x = encode_val(x, width);
  y = encode_val(y, height);

  //Move the cursor to the position
  conditional_homing();
  pos(x, y);
  update();
  delay_ms(tx_delay);

  hold();
}

// Travel (while already holding)
// (no homing)
void MouseDevice::travel(int16_t from_x, int16_t from_y, int16_t to_x, int16_t to_y, uint16_t duration) {
  //Check if duration is reasonable
  if(duration <= 100)  duration = 100;

  from_x = encode_val(from_x, width);
  from_y = encode_val(from_y, height);

  to_x = encode_val(to_x, width);
  to_y = encode_val(to_y, height);


  //Move the cursor to the start position
  pos(from_x, from_y);
  update();
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
      update();
      delay_ms(tx_delay);
    } while (progress < 1);

  //Snap to end coords
  pos(to_x, to_y);
  update();
  delay_ms(tx_delay);
}

// Release from MouseDevice::hold()
void MouseDevice::release() {
  //Button up
  set_left_button(false);
  update();
}

// Scroll using the mouse wheel
void MouseDevice::scroll(int16_t amount) {
  uint8_t sign = (amount > 0) ? -1 : 1; // Inverted, so that negative is up, positive down
  amount = abs(amount);

  //Scroll piece by piece until value is met
  while (amount > 0) {
    report[6] = min(127, amount);
    amount -= report[6];
    report[6] *= sign;
    update();
    delay_ms(tx_delay);
  }

  //Clear the scroll wheel, relative value needs to return to 0
  report[6] = 0;
  update();
  delay(tx_delay);
}
void MouseDevice::scroll(int16_t at_x, int16_t at_y, int16_t amount) {
  at_x = encode_val(at_x, width);
  at_y = encode_val(at_y, height);

  //Move the cursor to the position
  pos(at_x, at_y);
  update();
  delay_ms(tx_delay);

  scroll(amount);
}