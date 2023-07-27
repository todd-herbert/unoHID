#include "vusb_controller.h"

VUSBController::VUSBController(PollingTimer timer) {
    // Save the timer which was selected with macros in unoHID.h
    this->polling_timer = timer;
}

void VUSBController::begin() {
    
    // Setup Pins
    PORTD = 0;
    DDRD |= ~USBMASK;

    // Reconnect so device is detected
    cli();
    usbDeviceDisconnect();

    delay(1000);

    usbDeviceConnect();
    usbInit();
    sei();

    // Wait one second for enumeration to finish. (Android USB OTG requires >500ms)
    // (Polling every 10ms)
    for(int i=0; i<100; i++) {
        usbPoll();
        delay(10);
    }  

    // Setup the timer, if required, for infrequent "keep alive" polling

    if (polling_timer == Timer1) {
        // TIMER 1 for interrupt frequency 125 Hz:
        cli(); // stop interrupts
        TCCR1A = 0; // set entire TCCR1A register to 0
        TCCR1B = 0; // same for TCCR1B
        TCNT1  = 0; // initialize counter value to 0
        // set compare match register for 125 Hz increments
        OCR1A = 15999; // = 16000000 / (8 * 125) - 1 (must be <65536)
        // turn on CTC mode
        TCCR1B |= (1 << WGM12);
        // Set CS12, CS11 and CS10 bits for 8 prescaler
        TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
        // enable timer compare interrupt
        TIMSK1 |= (1 << OCIE1A);
        sei(); // allow interrupts
    }
    else if (polling_timer == Timer2) {
        // Timer 2 for interrupt frequency 100Hz - period 10ms
        cli();
        TCCR2A = 0;
        TCCR2B = 0;
        TCNT2 = 0;
        // 100.16025641025641 Hz (16000000/((155+1)*1024))
        OCR2A = 155;
        // CTC
        TCCR2A |= (1 << WGM21);
        // Prescaler 1024
        TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
        // Output Compare Match A Interrupt Enable
        TIMSK2 |= (1 << OCIE2A);
        sei();
    }
}

void VUSBController::end() {
    // Un-set timers

    if (polling_timer == Timer1) {
        cli();
        TIMSK1 = 0;
        sei();
    }
    else if (polling_timer == Timer2) {
        cli();
        TIMSK2 = 0;
        sei();
    }

    cli();
    usbDeviceDisconnect();
    sei();
}

void VUSBController::poll() {
    // No autopolling if we're actually doing something
    if(!autopolling_paused) {
        usbPoll();
    }
}

void VUSBController::mouseOff() {
    mouseEnabled = false;
    if (!keyboardEnabled)
        end();
}

void VUSBController::keyboardOff() {
    keyboardEnabled = false;
    if (!mouseEnabled)
        end();
}

void VUSBController::mouseOn() {
    mouseEnabled = true;
    if (!keyboardEnabled)
        begin();
}

void VUSBController::keyboardOn() {
    keyboardEnabled = true;
    if (!mouseEnabled)
        begin();
}

void VUSBController::pausePolling() {
    autopolling_paused = true;
}

void VUSBController::resumePolling() {
    autopolling_paused = false;
}