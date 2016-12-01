// Wave format file transfer demo for Arduino Mega over Ethernet
// Author: Bruno Senzio-Savino Barzellato
// V 1.0
// Notes: Transfer function can be optimized for filename and buffer size input
// December 1, 2016

// So far no data for this, decided to upload it for people that might consider this
// useful :)

//Using SdFat library
#include <SdFat.h>
SdFat sd;

#include <SPI.h>
#include <Ethernet2.h>

// SD chip select pin
const uint8_t chipSelect = 53; //Connect SD Card to pins 50, 51, 52 and 53 

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x10,0xCA,0x90 };

IPAddress server(10, 10, 6, 93); //SERVER IP ADDRESS

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(10,10,4,15); //ARDUINO IP ADDRESS

// Initialize the Ethernet client library
// with the IP address and port of the server
EthernetClient client;
int localport =19126;
// print stream
ArduinoOutStream cout(Serial);

//------------------------------------------------------------------------------
// store error strings in flash memory
#define error(s) sd.errorHalt_P(PSTR(s))
//------------------------------------------------------------------------------
void ftransfer() {

  char rbuffer[1024]; //Chunk can be increased to bigger sizes, be careful about memory!!
  char c;
  int n;
  // open test file
  SdFile rdfile("test.wav", O_READ); // <--- Name of file to be transfered

  //Obtain file size
  uint32_t fsize;
  fsize = rdfile.fileSize() - rdfile.curPosition();
  
  Serial.println(fsize);
  // check for open error
  if (!rdfile.isOpen()) error("File could not be opened, check configuration...");

    // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("Connecting to server for transfer...");
 Serial.println(Ethernet.localIP());
  // if you get a connection, report back via serial:
  if (client.connect(server, localport)) {
    Serial.println("Connected");

  while ((n = rdfile.fgets(rbuffer, sizeof(rbuffer))) > 0) {
       client.write(rbuffer, sizeof(rbuffer)-1);
  }
    
    client.stop();
  }
  else {
    // No response from server reply
    Serial.println("No server available");
  }

}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void setup(void) {

  //Allow communications for SD Card ant Ethernet Shield
  pinMode(10, OUTPUT);                       
  digitalWrite(10, HIGH);                    
  
  Serial.begin(9600);
  while (!Serial) {}  // Wait for Leonardo

  cout << pstr("Type any character to start\n");
  while (Serial.read() <= 0) {}
  delay(400);  // catch Due reset problem
  
  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) sd.initErrorHalt();
  
  ftransfer();
  
  cout << pstr("\nDone\n");
}

void loop(void) {}
