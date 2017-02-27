/*
* Testing the SPI bus
*
* This self contained test program will read registers 1, 2 and 3.
* If the module is working properly, it should generate the following output:
*
* SX1276 register readout:
* RegOpMode: 9
* RegBitrateMsb: 1A
* RegBitrateLsb: B
*
* (these are the initial values of the registers).
*
* Connections:
* PIN 7:  NSS
* PIN 11: MOSI
* PIN 12: MISO
* PIN 13: SCK
*/
#include <SPI.h>

const byte SX1276_NSS = 7;

/* Write to the LoRa chip
* addr: initial register to write to
* data: what to write (this buffer will be overwritten)
* count: the number of registers to write
*/
void SX1276_write(byte addr, byte *data, byte count)
{
  addr |= 0x80;  // Write: bit 7 is 1
  SX1276_transfer(addr, data, count);
}

/* Read from the LoRa chip
* addr: initial register to read
* data: buffer to store the data in
* count: the number of registers to read
*/
void SX1276_read(byte addr, byte *data, byte count)
{
  addr &= 0x7F;  // Read: bit 7 is 0
  SX1276_transfer(addr, data, count);
}

/* Utility function for reading and writing
* addr: initial register to read or write
* data: buffer contains data to be sent, overwritten with data that is read back
* count: the number of registers to read/write
*/
void SX1276_transfer(byte addr, byte *data, byte count)
{
  SPI.beginTransaction(SPISettings(4000000,MSBFIRST,SPI_MODE0));
  digitalWrite(SX1276_NSS, 0);
  SPI.transfer(addr);
  SPI.transfer(data, count);
  digitalWrite(SX1276_NSS, 1);
  SPI.endTransaction();
}

/* Writing a single register
* addr: register to write
* data: new value
*/
void SX1276_write(byte addr, byte data)
{
  SX1276_write(addr, &data, 1);
}


/* Reading a single register
* addr: register to read
* 
* Returns: value of register
*/
byte SX1276_read(byte addr)
{
  byte data = 0;
  SX1276_read(addr, &data, 1);
  return data;
}

void setup()
{
  pinMode(SX1276_NSS, OUTPUT);
  SPI.begin();
  
  Serial.begin(9600);
  Serial.println("SX1276 register readout:");
  byte input[3];
  SX1276_read(0x01, input, sizeof(input));
  Serial.print("RegOpMode: ");
  Serial.println(input[0], HEX);
  Serial.print("RegBitrateMsb: ");
  Serial.println(input[1], HEX);
  Serial.print("RegBitrateLsb: ");
  Serial.println(input[2], HEX);
}

void loop() {
  // put your main code here, to run repeatedly:

}
