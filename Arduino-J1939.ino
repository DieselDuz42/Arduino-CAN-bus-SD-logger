/*
   Arduino J1939
  http://github.com/DieselDuz42/Arduino-J1939

*/

#include <mcp_can.h>
#include <SPI.h>
#include <SD.h>

/* Assign pins */
#define CAN0_INT 2 
#define SD_CHIP_SELECT 9
MCP_CAN CAN0(10);
#define SERIAL_ON 1


long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];
unsigned long lastmsgrcv = 0;
unsigned long tdiff = 0;
char tbuff;

/* SD card Settings */
char fileName[]     = "DATA00.txt";
char header[]       = "Time,Time Diff,ID,DLC,Data";
File dataFile;

boolean initSD(void)
{
  pinMode(SD_CHIP_SELECT, OUTPUT);
  if (!SD.begin(SD_CHIP_SELECT))
  {
    #if SERIAL_ON
      Serial.println(F("Card failed, or not present"));
	  #endif
  }
  else
  {
    #if SERIAL_ON
	    Serial.println(F("Card initialized."));
	  #endif
  }
}


/* Timing function */
unsigned long gettdiff()
{
  unsigned long curr_t = millis();
  unsigned long res = curr_t - lastmsgrcv;
  return ( res );
}


void setup()
{
  Serial.begin(115200);
  initSD();
  #if SERIAL_ON
    Serial.println(F("Beginning to log"));
    Serial.print(F("Checking file name: "));
    Serial.println(fileName);
  #endif
  boolean fileExists = SD.exists(fileName);
  if ( fileExists == true )
  {
    for (uint8_t i = 0; i < 100; i++)
    {
      fileName[4] = i / 10 + '0';
      fileName[5] = i % 10 + '0';
      #if SERIAL_ON
	      Serial.print(F("Checking file name: "));
        Serial.println(fileName);
	    #endif
      fileExists = SD.exists(fileName);
      if (fileExists == false )
      {
        break;
      }
    }
  }
  else
  {
    //file does not exist yet -> create it
  }
  dataFile = SD.open(fileName, FILE_WRITE);
  dataFile.println(header);
  dataFile.flush();

  delay(2000); // running into issues powering on with bus traffic and not logging
  // Initialize MCP2515 running at 16MHz with a baudrate of 250kb/s and the masks and filters disabled.
  if (CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ) == CAN_OK)
    #if SERIAL_ON
      Serial.println(F("MCP2515 Initialized Successfully!"));
    else
      Serial.println(F("Error Initializing MCP2515..."));
	#endif
	
  CAN0.setMode(MCP_LISTENONLY);                     
  pinMode(CAN0_INT, INPUT);                            
  #if SERIAL_ON
    Serial.println(F("MCP2515 Ready"));
    boolean setupSuccess = true;
  #endif
}

void loop()
{

  if (!digitalRead(CAN0_INT))                        
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    if (&rxId>0)
    {
      lastmsgrcv = millis();          // set the last recived var
      tdiff = gettdiff();            // set the time difference
      //sprintf(tbuff, "%ld,%ld,", millis(), tdiff); // add the times to the message
      //#if SERIAL_ON
  	  //Serial.print(tbuff);
	    //#endif
      //dataFile.print(tbuff);
      sprintf(msgString, "%ld,%ld,0x%.8lX,%1d,", millis(), tdiff, (rxId & 0x1FFFFFFF), len); // formats the message
	    #if SERIAL_ON
	      Serial.print(msgString);        // print the compiled message to serial
	    #endif
      dataFile.print(msgString);      // print the compiled message to file
      for (byte i = 0; i < len; i++)
        {    // loop through the data buffer
          sprintf(msgString, "0x%.2X,", rxBuf[i]);// format the data buffer
          #if SERIAL_ON
	          Serial.print(msgString);        // print the formatted data to serial
	        #endif
          dataFile.print(msgString);        // print the formatted data to file
        }
    
  
  #if SERIAL_ON
    Serial.println();         // prints newline to serial
  #endif
  dataFile.println();           // prints newline to file
    }
  }
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
