/*
   Arduino J1939
  http://github.com/DieselDuz42/Arduino-J1939

*/

#include <mcp_can.h>
#include <SPI.h>
#include <SD.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                          // Array to store serial string
unsigned long lastmsgrcv = 0;         // Time the last message was received
unsigned long tdiff = 0;            // Difference in time for messages
char tbuff[8];                       // Store timing information

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10
const int SD_CHIP_SELECT       = 9;       // Set SD_CHIP_SELECT to pin 9

char fileName[]     = "D00.txt";        // Filename for the log file
char header[]       = "Time,Time Diff, ID,DLC, Data";
File dataFile;                  // File object used to access the SD card

boolean initSD(void)              // Initialize the SD card
{
  boolean setupSuccess = false;
  // Initialize the SD card SPI CS Pin
  pinMode(SD_CHIP_SELECT, OUTPUT);
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
unsigned long gettdiff()        // Function to get the time between messages
{
  unsigned long curr_t = millis();
  unsigned long res = curr_t - lastmsgrcv;
  return ( res );
}


void setup()
{

  Serial.begin(115200);


  // Initialize MCP2515 running at 16MHz with a baudrate of 250kb/s and the masks and filters disabled.
  if (CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ) == CAN_OK)
   Serial.println("MCP2515 Initialized Successfully!");
  else
   Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_LISTENONLY);                     // Listen only because we do not want to disturb the vehicle's

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
  if ( fileExists == true )
  {
    for (uint8_t i = 0; i < 100; i++)
    {
      fileName[4] = i / 10 + '0';
      fileName[5] = i % 10 + '0';
     Serial.print("Checking file name: ");
     Serial.println(fileName);
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
  //Open file and get ready to enter data
  dataFile = SD.open(fileName, FILE_WRITE);
  //Write the header line
  dataFile.println(header);
  dataFile.flush();


}

void loop()
{

  if (!digitalRead(CAN0_INT))                        // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    tdiff = gettdiff();            // set the time difference
    lastmsgrcv = millis();          // set the last recived var
    sprintf(tbuff, "Time: %ld : Diff: %ld ", millis(), tdiff); // add the times to the message
   Serial.print(tbuff);
    dataFile.print(tbuff);
    sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len); // formats the message


   Serial.print(msgString);        // print the compiled message to serial
    dataFile.print(msgString);      // print the compiled message to file


    for (byte i = 0; i < len; i++) {    // loop through the data buffer
      sprintf(msgString, " 0x%.2X", rxBuf[i]);// format the data buffer
     Serial.print(msgString);        // print the formatted data to serial
      dataFile.print(msgString);        // print the formatted data to file
    }
  }

  Serial.println();         // prints newline to serial
  dataFile.println();           // prints newline to file
  //flush for illustration purpose only
  dataFile.flush();
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

