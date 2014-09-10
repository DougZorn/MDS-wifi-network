#include "cc2500_REG.h"
#include "cc2500_VAL.h"
#include "cc2500init.h"
#include "read_write.h"
#include <QueueList.h>
#include <SPI.h>
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
	
	long previousTXTimeoutMillis = 0;
	pinMode(9, OUTPUT);
	digitalWrite(9, LOW);	
}

#define SEND_HANDSHAKE_PACKET 0x01
#define WAIT_4_ACK 0x02
#define LISTEN_4_PACKET 0x03
byte state = INITIAL_STATE;
byte next_state = 0;
byte packet[] = {0x01, 0xFF};

void loop()
{ 
	switch(state)
	{
		case SEND_HANDSHAKE_PACKET:
			Serial.println("Send State 1");
			sendFixedPacket(packet,2);
			Serial.println("Send State 2");
			digitalWrite(9, HIGH);
			state = WAIT_4_ACK;
			break;
		case WAIT_4_ACK:
			Serial.println("Waiting for ack");
			if(listenForPacket(&myQueue)) //listenForPacket is blocking function that has timeout built in as return value 0
			{
				Serial.println("ACK received");
				state = LISTEN_4_PACKET;
				digitalWrite(9, LOW);
			}
			else
			{
				state = SEND_HANDSHAKE_PACKET;
			}			
			break;
		case LISTEN_4_PACKET:
			Serial.println("Listening for packet");
			listenForPacket(&myQueue);			
			break;	
	}  
} 

