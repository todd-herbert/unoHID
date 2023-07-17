/*
 * This file describes a digitizer / graphics tablet device, along with supporting methods.
 * The included VUSB library will perform the appropriate USB communications.
 */

#ifndef __STYLUS_H__
#define __STYLUS_H__

#include <Arduino.h>
#include "vusb/usbdrv.h"

class HybridDevice {
  //Methods
  public:
    
    // Pseudo constructors
    void begin(uint16_t width, uint16_t height);
    void begin();    //If unspecified, use percentage (100, 100)

    // Configure byte 1 of report_mouse
    void set_left_button    (bool state) { bitWrite(report_mouse[1], 0, state); }
    void set_right_button   (bool state) { bitWrite(report_mouse[1], 1, state); }
    void set_middle_button  (bool state) { bitWrite(report_mouse[1], 2, state); }

    // High level methods
    void set_dimensions(uint16_t width, uint16_t height);
    void perform_homing();
    void click(int16_t x, int16_t y);
    void middle_click(int16_t x, int16_t y);
    void right_click(int16_t x, int16_t y);
    void double_click(int16_t x, int16_t y);
    void long_press(int16_t x, int16_t y, uint16_t duration);
    void drag(int16_t from_x, int16_t from_y, int16_t to_x, int16_t to_y, uint16_t duration);
    void hold(int16_t x, int16_t y);
    void travel(int16_t from_x, int16_t from_y, int16_t to_x, int16_t to_y, uint16_t duration);
    void position(int16_t x, int16_t y);
    void release();
    void scroll(int16_t at_x, int16_t at_y, int16_t amount);

  private:
    // Configure byte 1 of report_stylus
    void set_tip_switch     (bool state) { bitWrite(report_stylus[1], 0, state); }
    void set_in_range       (bool state) { bitWrite(report_stylus[1], 5, state); }

    void pos(uint16_t x, uint16_t y);                   // Bytes 2 - 5 of report_stylus, xy coords
    void update_stylus();                                      // Send stylus report_stylus
    void update_mouse();
    int16_t encode_val(int16_t val, uint16_t op_edge);     // Encode coordinates for the HID report_stylus

    // Alternative delay method. Avoid Timer0 during critical USB functions
    void delay_ms(unsigned int ms) {
      for (unsigned int i = 0; i < ms; i++) {
        delayMicroseconds(1000);
      }
    }

  // Members
  public:
    uint8_t report_mouse[7] = {0x01, 0, 0, 0, 0, 0, 0}; //Bit 0 is ReportID 1, to show that we're sending mouse data
    uint8_t report_stylus[6] = {0x02, 0, 0, 0, 0, 0};  //Bit 0 is ReportID 2, to show that we're sending stylus data
    
  private:
    const uint8_t tx_delay = 20;
    uint16_t width = 100;
    uint16_t height = 100;
    
};

#endif //__ABSOLUTE_MOEMULATE_STYLUS_H__