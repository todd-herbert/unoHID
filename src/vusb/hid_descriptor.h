#ifndef __HID_DESCRIPTOR_H__
#define __HID_DESCRIPTOR_H__

// This dataset describes our imaginary stylus / mouse device to the USB host, via the VUSB driver


PROGMEM const uint8_t usbHidReportDescriptor[] =  { 
    //Mouse Report
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //  Report ID 1    
    0x09, 0x01,                    //   USAGE (Pointer)
    0xA1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM
    0x29, 0x03,                    //     USAGE_MAXIMUM
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Const,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x75, 0x10,                    //     REPORT_SIZE (16)                                                  
    0x26, 0xFF, 0x7F,              //     LOGICAL_MAXIMUM (32767)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs) 
    0x09, 0x31,                    //     USAGE (Y)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs) 
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7F,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xC0,                          //   END_COLLECTION
    0xC0,                          // END COLLECTION

    // Stylus Report
    0x05, 0x0d,                         // USAGE_PAGE (Digitizers)          
    0x09, 0x02,                         // USAGE (Pen)                      
    0xa1, 0x01,                         // COLLECTION (Application)      
    0x85, 0x02,                         //  Report ID 2            
    0x09, 0x20,                         //   USAGE (Stylus)                 
    0xa1, 0x00,                         //   COLLECTION (Physical)  
    0x15, 0x00,                         //     LOGICAL_MINIMUM (0)          
    0x25, 0x01,                         //     LOGICAL_MAXIMUM (1)          
    0x75, 0x01,                         //     REPORT_SIZE (1)              
    0x95, 0x01,                         //     REPORT_COUNT (1)          
    0x09, 0x42,                         //     USAGE (Tip Switch)
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)             
    0x09, 0x44,                         //     USAGE (Barrel Switch)     
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)     
    0x09, 0x3c,                         //     USAGE (Invert)          
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)       
    0x09, 0x45,                         //     USAGE (Eraser Switch)                   
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)                   
    0x81, 0x03,                         //     INPUT (Cnst,Var,Abs)         
    0x09, 0x32,                         //     USAGE (In Range)             
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)         
    0x95, 0x02,                         //     REPORT_COUNT (2)             
    0x81, 0x03,                         //     INPUT (Cnst,Var,Abs)         
    0x05, 0x01,                         //     USAGE_PAGE (Generic Desktop) 
    0x09, 0x30,                         //     USAGE (X)                    
    0x75, 0x10,                         //     REPORT_SIZE (16)             
    0x95, 0x01,                         //     REPORT_COUNT (1)                                       
    0x26, 0xFF, 0x7F,                   //     LOGICAL_MAXIMUM (32767)      
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)         
    0x09, 0x31,                         //     USAGE (Y)                        
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)                           
    0xC0,                          //   END_COLLECTION              
    0xc0,                          // END_COLLECTION
};

#endif //__HID_DESCRIPTOR_H__