# Arduino-J1939
I have an Arduino, and an interest in J1939.

Hopefully this will end up with a better understanding and a better way to diagnose a vehicle's J1939 can bus.

## End Goal

Have an Arduino powered device that connects to a heavy duty vehicle and displays any known issues with the physical bus, or with the devices on the bus.

## Known issues

The Arduino Uno lacks in processing power & memory. - It's what I have now.

Writing information to SD card takes time. - I need the ability to review the data later, it can't slow down my day job.

My code is horrible - I'm working on that.



### Hardware
* [Arduino Uno](http://www.banggood.com/UNO-R3-ATmega328P-Development-Board-For-Arduino-No-Cable-p-964163.html)

* [Can Bus Shield](https://www.aliexpress.com/item/CAN-BUS-Shield-for-Arduino/32569554666.html)

* [SD Card](https://www.amazon.com/Professional-SanDisk-MicroSDHC-requirements-recordings/dp/9791299625/)


### Libraries used
* MCP_CAN: https://github.com/coryjfowler/MCP_CAN_lib
* SPI
* SD
