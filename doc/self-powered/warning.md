## DevKit Precautions
Many users of the DevKit sketch will be connecting their Arduino's Serial output to one machine, and the USB output to a separate target machine.

Some devices, such as mobile phones, will welcome power on the 5V line. Other devices may not have been explicitly designed for this.

It is advised that you do not connect 5V from the Arduino to your Target machine, only from your debugging machine to the Arduino.

This removes the potential for a slightly imbalance in voltage between the two machines to cause one 5V supply to "drive" the other.

I do not believe that this is a critical issue; I have on occasion connected both devices simultaneously without any apparent issues. I am not knowledgable enough to make a definitive statement on this matter, so I choose to err on the side of caution.

This schematic outlines the suggested wiring for a "self-powered" VUSB device (*i.e. the DevKit*)

![schematic for self powered vusb](./self-powered-schematic.jpg)