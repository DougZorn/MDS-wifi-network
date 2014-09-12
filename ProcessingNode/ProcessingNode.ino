#include "cc2500_REG.h"
#include "cc2500_VAL.h"
#include "cc2500init.h"
#include "read_write.h"
#include <SPI.h>
#include <QueueList.h>
#include "node_config.h"

QueueList<byte> packet;

void setup() {
	Serial.begin(9600);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setDataMode(SPI_MODE0);  
	SPI.begin();  
	SendStrobe(CC2500_SRES);
	init_CC2500_V2();
	pinMode(9, OUTPUT);
	digitalWrite(9, LOW);
}

void loop() {
	if(listenForPacket(&packet))
	{
		if(packet.count()>=8)
		{
			while(!packet.isEmpty())
			{
				Serial.print(packet.pop());
				Serial.print(" ");
			}
			Serial.println();			
		}
		else
		{
			EmptyQueueList(&packet);
		}
	}  
}
