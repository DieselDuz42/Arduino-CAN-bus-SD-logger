/*
* Arduino J1939
*
*	Hardware:
*		Arduino Uno: http://www.banggood.com/UNO-R3-ATmega328P-Development-Board-For-Arduino-No-Cable-p-964163.html
*		Can Bus Shield: https://www.aliexpress.com/item/CAN-BUS-Shield-for-Arduino/32569554666.html
*		SD Card: https://www.amazon.com/Professional-SanDisk-MicroSDHC-requirements-recordings/dp/9791299625/
*	Libraries used:
*		MCP_CAN: https://github.com/coryjfowler/MCP_CAN_lib
*	Examples used:
*		Arduino can logger: http://tucrrc.utulsa.edu/Publications/Arduino/arduino.html
*		CAN_RECIEVE example from the MCP_CAN library
*/

#include <mcp_can.h>
#include <SPI.h>
#include <SD.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10
/** SD_CHIP_SELECT assigned to pin 9*/
const int SD_CHIP_SELECT       = 9;

/**File name of log file*/
char fileName[]     = "DATA00.txt";
/**Column headers for logged data*/
char header[]       = "Msg#,Time Diff, ID,DLC, Data";
/**File objet used to access the SD card*/
File dataFile;

/**Initializes SD card communication*/
boolean initSD(void)
{
  boolean setupSuccess =false;
  // Initialize the SD card SPI CS Pin
  pinMode(SD_CHIP_SELECT, OUTPUT);      
  //digitalWrite(SD_CHIP_SELECT, HIGH);   // Turns Sd Card communication off 
  //Check whether SD card initialization is successful
  if (!SD.begin(SD_CHIP_SELECT)) 
  {  
    Serial.println("Card failed, or not present");
  }
  else
  {
      Serial.println("Card initialized.");
      setupSuccess = true;
  }
  return setupSuccess;
}

void setup()
{
  
  Serial.begin(115200);

  
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_LISTENONLY);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  
  Serial.println("MCP2515 Library Receive Example...");
  boolean setupSuccess = true;
  //sd
  setupSuccess = initSD();
  
  if ( setupSuccess == true)
  { 
    Serial.println("Beginning to log");
  }

  //Find the next available file name to prevent overwriting previously recorded sessions.
  Serial.print("Checking file name: ");
  Serial.println(fileName); 
  boolean fileExists = SD.exists(fileName);
  //If the file exists change the name until a new one is created
  if( fileExists == true )
  {
    for (uint8_t i = 0; i < 100; i++) 
    {
      fileName[4] = i/10 + '0';
      fileName[5] = i%10 + '0';
      Serial.print("Checking file name: ");
      Serial.println(fileName);
      fileExists = SD.exists(fileName);
      if(fileExists == false )
      {
        break;
      }
    }
   }
   else
   {
     //file does not exist yet -> create it
   }
  //Open file and get ready to enter data
  dataFile = SD.open(fileName, FILE_WRITE);
  //Write the header line
  dataFile.println(header);
  dataFile.flush();
  
  
}

void loop()
{

  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
	  // print the message
	  Serial.print(msgString);
      dataFile.print(msgString);
    } else {
	  // loop through the data buffer
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
        dataFile.print(msgString);
      }
    }
        
    Serial.println();
    dataFile.println();     
    //flush for illustration purpose only
    dataFile.flush();
  }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
