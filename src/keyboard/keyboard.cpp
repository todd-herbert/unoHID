/*
  Keyboard.cpp

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "keyboard.h"
#include "KeyboardLayout.h"
#include "Keyboard_de_DE.h"

Keyboard_::Keyboard_(VUSBController *vusb) {
    _asciimap = KeyboardLayout_en_US;

    // Store reference to the VUSB controller
    this->vusb = vusb;
}

void Keyboard_::begin(const uint8_t *layout) {
    _asciimap = layout;

    // Tell the VUSB controller to start up, if not already started by mouse
    vusb->keyboardOn();
}

void Keyboard_::end(void) {
    // VUSB shutdown, if mouse not also active
    vusb->keyboardOff();
}


void Keyboard_::sendReport(KeyReport* keys) {
    // This method rewritten to use VUSB
    vusb->pausePolling();   // No auto-polling with timer

    // See if we are ready to use USB
    for ( uint16_t i = 0; i < 5000; i += 15 ) {
        usbPoll();
        if (usbInterruptIsReady()) {
            // Send the report
            usbSetInterrupt( (uchar*) keys, 8);
            break;
        }
        delay(15);
    }

    delay(tx_delay);
    vusb->resumePolling();
}

uint8_t USBPutChar(uint8_t c);

// press() adds the specified key (printing, non-printing, or modifier)
// to the persistent key report and sends the report.  Because of the way
// USB HID works, the host acts like the key remains pressed until we
// call release(), releaseAll(), or otherwise clear the report and resend.
size_t Keyboard_::press(uint8_t k) {
    uint8_t i;
    if (k >= 136) {         // it's a non-printing key (not a modifier)
        k = k - 136;
    } else if (k >= 128) {  // it's a modifier key
        _keyReport.modifiers |= (1<<(k-128));
        k = 0;
    } else {                // it's a printing key
        k = pgm_read_byte(_asciimap + k);
        if (!k) {
            setWriteError();
            return 0;
        }
        if ((k & ALT_GR) == ALT_GR) {
            _keyReport.modifiers |= 0x40;   // AltGr = right Alt
            k &= 0x3F;
        } else if ((k & SHIFT) == SHIFT) {
            _keyReport.modifiers |= 0x02;   // the left shift modifier
            k &= 0x7F;
        }
        if (k == ISO_REPLACEMENT) {
            k = ISO_KEY;
        }
    }

    // Add k to the key report only if it's not already present
    // and if there is an empty slot.
    if (_keyReport.keys[0] != k && _keyReport.keys[1] != k &&
        _keyReport.keys[2] != k && _keyReport.keys[3] != k &&
        _keyReport.keys[4] != k) {

        for (i=0; i<5; i++) {
            if (_keyReport.keys[i] == 0x00) {
                _keyReport.keys[i] = k;
                break;
            }
        }
        if (i == 5) {
            setWriteError();
            return 0;
        }
    }

    sendReport(&_keyReport);
    return 1;
}

// release() takes the specified key out of the persistent key report and
// sends the report.  This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.
size_t Keyboard_::release(uint8_t k) {
    uint8_t i;
    if (k >= 136) {         // it's a non-printing key (not a modifier)
        k = k - 136;
    } else if (k >= 128) {  // it's a modifier key
        _keyReport.modifiers &= ~(1<<(k-128));
        k = 0;
    } else {                // it's a printing key
        k = pgm_read_byte(_asciimap + k);
        if (!k) {
            return 0;
        }
        if ((k & ALT_GR) == ALT_GR) {
            _keyReport.modifiers &= ~(0x40);   // AltGr = right Alt
            k &= 0x3F;
        } else if ((k & SHIFT) == SHIFT) {
            _keyReport.modifiers &= ~(0x02);    // the left shift modifier
            k &= 0x7F;
        }
        if (k == ISO_REPLACEMENT) {
            k = ISO_KEY;
        }
    }

    // Test the key report to see if k is present.  Clear it if it exists.
    // Check all positions in case the key is present more than once (which it shouldn't be)
    for (i=0; i<5; i++) {
        if (0 != k && _keyReport.keys[i] == k) {
            _keyReport.keys[i] = 0x00;
        }
    }

    sendReport(&_keyReport);
    return 1;
}

void Keyboard_::releaseAll(void) {
    _keyReport.keys[0] = 0;
    _keyReport.keys[1] = 0;
    _keyReport.keys[2] = 0;
    _keyReport.keys[3] = 0;
    _keyReport.keys[4] = 0;
    _keyReport.modifiers = 0;
    sendReport(&_keyReport);
}

size_t Keyboard_::write(uint8_t c) {
    uint8_t p = press(c);   // Keydown
    release(c);     // Keyup
    return p;       // just return the result of press() since release() almost always returns 1
}

size_t Keyboard_::write(const uint8_t *buffer, size_t size) {
    size_t n = 0;
    while (size--) {
        if (*buffer != '\r') {
            if (write(*buffer)) {
                n++;
            } else {
                break;
            }
        }
        buffer++;
    }
    return n;
}

void Keyboard_::setTxDelay(uint16_t delay) {
    this->tx_delay = delay;
}
