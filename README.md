# Arduino CAN bus logger
Using an Arduino and a CAN shield connected to a vehicle, save as much information as possible to a SD card for processing later. 


## Useage
You can control serial output and the ability to have the Arduino record the time between messages.
Setting SERIAL_ON to 1 enables serial output and doesn't take up extra memory thanks to the F() function.
Setting TIMING_ON to 1 enables the processing and display of time between messages. Currently this uses 18 bytes when enabled.

## Known issues

Currently using the Arduino Leonardo, I have a Mega and a Due on the way to play with.

Writing information to SD card takes time. - I need the ability to review the data later, it can't slow down my day job.

My code is horrible - I'm working on that. I have limited programming expierence and even less using GitHub or Arduino.




### Hardware
* [Arduino Leonardo] (https://www.aliexpress.com/item/2012-Official-Leonardo-R3-MCU-Board-ATmega32u4-Board-with-Original-IC-for-Arduino-Compatible-DIY-Electronics/32602077173.html) Upgraded to the Leonardo for an extra 512bytes of SRAM. 5/1/17
* [Arduino Uno](http://www.banggood.com/UNO-R3-ATmega328P-Development-Board-For-Arduino-No-Cable-p-964163.html)

* [Can Bus Shield](https://www.aliexpress.com/item/CAN-BUS-Shield-for-Arduino/32569554666.html)

* [SD Card](https://www.amazon.com/Professional-SanDisk-MicroSDHC-requirements-recordings/dp/9791299625/)


### Libraries used
* MCP_CAN: https://github.com/coryjfowler/MCP_CAN_lib
* SPI
* SD
