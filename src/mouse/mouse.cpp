#include "mouse/mouse.h"

// Grab reference to the controller, so we can call VUSBController::mouseOn(), etc
MouseDevice::MouseDevice( VUSBController *vusb ) {
    this->vusb_controller = vusb;
}


void MouseDevice::begin() {
    // Let the controller decide whether vusb should init
    vusb_controller->mouseOn();
}


void MouseDevice::end() {
    // Disconnect USB, if keyboard isn't also enabled
    vusb_controller->mouseOff();
}

void MouseDevice::setTxDelay(uint16_t delay) {
    this->tx_delay = delay;
}


bool MouseDevice:: isPressed(MouseButton button) {
    return bitRead(report[1], button - 1);
}


void MouseDevice::setButton(MouseButton button, bool state) {
    // Mouse Button 1 is bit 0  - to match standard naming convention
    bitWrite(report[1], button - 1, state);

    // Clear any old movement from the report
    report[2] = 0;
    report[3] = 0;
    report[4] = 0;
    report[5] = 0;
    report[6] = 0;
}


// Send the report (Tell host what our mouse is doing)
void MouseDevice::update() {
    vusb_controller->pausePolling();    // No auto-polling with timer

    // See if we are ready to use USB
    for ( uint16_t i = 0; i < 5000; i += 15 ) {
        usbPoll();
        if (usbInterruptIsReady()) {
            // Send the report
            usbSetInterrupt(report, sizeof(report));
            break;
        }
        delay(15);
    }
    
    delay(tx_delay);
    vusb_controller->resumePolling();
}


void MouseDevice::click( MouseButton button ) {
    
    //Button down
    setButton(button, true);
    update();

    //Pause for click to register
    delay(20);

    //Button up
    setButton(button, false);
    update();
}


void MouseDevice::move(int16_t x, int16_t y, int8_t wheel) {
    report[2] = x & 0xFF;
    report[3] = x >> 8;
    report[4] = y & 0xFF;
    report[5] = y >> 8;
    report[6] = wheel;

    update();
}


void MouseDevice::press(MouseButton button) {
    setButton(button, true);
    update();
}


void MouseDevice::release(MouseButton button) {
    setButton(button, false);
    update();
}


void MouseDevice::doubleClick(MouseButton button) {
    click(button);
    delay(100);
    click(button);
}



// Long click at the specified position
void MouseDevice::longClick(uint16_t duration, MouseButton button) {
    press(button);
    delay(duration);
    release(button);
}


// Scroll using the mouse wheel
void MouseDevice::scroll(int16_t amount) {
    uint8_t sign = (amount > 0) ? 1 : -1; 
    amount = abs(amount);

    // Clear any old movement from the report
    report[2] = 0;
    report[3] = 0;
    report[4] = 0;
    report[5] = 0;
    report[6] = 0;

    //Scroll piece by piece until value is met
    while (amount > 0) {
        report[6] = min(127, amount);
        amount -= report[6];
        report[6] *= sign;
        update();
    }

    //Clear the scroll wheel, relative value needs to return to 0
    report[6] = 0;
    update();
}