#include "cc2500_REG.h"
#include "cc2500_VAL.h"
#include "cc2500init.h"
#include "read_write.h"
#include <SPI.h>
#include <QueueList.h>
#include "node_config.h"

QueueList <byte> myQueue;

void setup()
{
  Serial.begin(9600);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE0);  
  SPI.begin();  
  SendStrobe(CC2500_SRES);
  init_CC2500_V2();    
} 

#define Bpacket 0x01
#define Npacket 0x02
#define Tpacket 0x03

byte BroadcastPacket[] = {0x04,0xFF,0x01,NODE_ID_ADDRESS,Bpacket};
byte HandShakeNextBroadcast[] = {0x02,NEXT_NODE_ADDRESS,Npacket};
byte HandShakeNextTransmit[] = {0x02,NEXT_NODE_ADDRESS,Tpacket};


byte state = INITIAL_STATE;
#define LISTEN_4_PACKET 0x01
#define BROADCAST 0x02
byte packet[] = {0x01, 0xFF};

void loop()
{
	switch(state)
	{
		case LISTEN_4_PACKET:
			if(listenForPacket(&myQueue))	
			{
				state = BROADCAST;
			}
			break;
		case BROADCAST:
			sendFixedPacket(packet,2);
			state = LISTEN_4_PACKET;
			break;
	}
} 


