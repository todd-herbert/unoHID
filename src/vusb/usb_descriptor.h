#ifndef __HID_DESCRIPTOR_H__
#define __HID_DESCRIPTOR_H__

// USB Device Name
// ------------------

// Max length: 18
// Please read https://github.com/todd-herbert/unoHID/blob/main/doc/vusb/USB-IDs-for-free.txt

#define USB_CFG_VENDOR_NAME_LEN     18
#define USB_CFG_VENDOR_NAME         't','i','n','y','u','r','l','.','c','o','m','/','u','n','o','H','I','D'

#define USB_CFG_DEVICE_NAME_LEN     6
#define USB_CFG_DEVICE_NAME         'U', 'n', 'o', 'H', 'I', 'D'


// This describes (to the target device) the format 
// which the Arduino will use to send mouse and keyboard data
// ----------------------------------------------------------

PROGMEM const uint8_t usbHidReportDescriptor[] =  { 
    // Mouse Report
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //   Report ID 1    
    0x09, 0x01,                    //   USAGE (Pointer)
    0xA1, 0x00,                    //       COLLECTION (Physical)
    0x05, 0x09,                    //       USAGE_PAGE (Button)
    0x19, 0x01,                    //           USAGE_MINIMUM
    0x29, 0x03,                    //           USAGE_MAXIMUM
    0x15, 0x00,                    //           LOGICAL_MINIMUM (0) -   Mouse Buttons
    0x25, 0x01,                    //           LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //           REPORT_COUNT (3)
    0x75, 0x01,                    //           REPORT_SIZE (1)
    0x81, 0x02,                    //           INPUT (Data,Var,Abs)
    0x95, 0x01,                    //           REPORT_COUNT (1)    -   Pad the byte
    0x75, 0x05,                    //           REPORT_SIZE (5)
    0x81, 0x03,                    //           INPUT (Const,Var,Abs)
    0x05, 0x01,                    //      USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //      USAGE (X)
    0x75, 0x10,                    //           REPORT_SIZE (16)
    0x16, 0x00, 0x80,              //           LOGICAL_MININIMUM (-32768)                              
    0x26, 0xFF, 0x7F,              //           LOGICAL_MAXIMUM (32767)
    0x81, 0x06,                    //           INPUT (Data,Var,Rel) 
    0x09, 0x31,                    //     USAGE (Y)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel) 
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7F,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xC0,                          //       END_COLLECTION
    0xC0,                          // END COLLECTION

    // Keyboard report to go here

    // 0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    // 0x09, 0x06,                    // USAGE (Keyboard)
    // 0xa1, 0x01,                    // COLLECTION (Application)
    // 0x85, 0x02,                    //  Report ID 2 
    // 0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    // 0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    // 0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    // 0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    // 0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    // 0x75, 0x01,                    //   REPORT_SIZE (1)
    // 0x95, 0x08,                    //   REPORT_COUNT (8)
    // 0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    // 0x95, 0x01,                    //   REPORT_COUNT (1)
    // 0x75, 0x08,                    //   REPORT_SIZE (8)
    // 0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    // 0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    // 0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    // 0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    // 0xc0                           // END_COLLECTION

 //  Arduino default keyboard library
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 47
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)

    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)

    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)

    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x73,                    //   LOGICAL_MAXIMUM (115)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)

    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x73,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0,                          // END_COLLECTION
};

#endif //__HID_DESCRIPTOR_H__