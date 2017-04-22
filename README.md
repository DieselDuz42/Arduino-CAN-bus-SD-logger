# Arduino CAN bus logger
Using an Arduino and a CAN shield connected to a vehicle, save as much information as possible to a SD card for processing later. 



## Known issues

The Arduino Uno lacks in processing power & memory. - It's what I have now.

Writing information to SD card takes time. - I need the ability to review the data later, it can't slow down my day job.

My code is horrible - I'm working on that. I have limited programming expierence and even less using GitHub or Arduino.




### Hardware
* [Arduino Uno](http://www.banggood.com/UNO-R3-ATmega328P-Development-Board-For-Arduino-No-Cable-p-964163.html)

* [Can Bus Shield](https://www.aliexpress.com/item/CAN-BUS-Shield-for-Arduino/32569554666.html)

* [SD Card](https://www.amazon.com/Professional-SanDisk-MicroSDHC-requirements-recordings/dp/9791299625/)


### Libraries used
* MCP_CAN: https://github.com/coryjfowler/MCP_CAN_lib
* SPI
* SD
