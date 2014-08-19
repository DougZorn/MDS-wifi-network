#include "cc2500_REG.h"
#include "cc2500_VAL.h"
#include "cc2500init.h"
#include "read_write.h"
#include <SPI.h>

int Packet[9]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00};
void setup()
{
  Serial.begin(9600);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE0);  
  SPI.begin();  
  SendStrobe(CC2500_SRES);
  init_CC2500_V2();  
}

void loop()
{  
	listenForPacket(Packet);
} 


