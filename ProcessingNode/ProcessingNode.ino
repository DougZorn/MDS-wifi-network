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
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
	SendStrobe(CC2500_SRES);
	init_CC2500_V2();
}

void loop() {
	if(listenForPacket(&packet))
	{
		if(packet.count()>=8)
		{
			packet.pop();
			packet.pop();
			packet.pop();
			while(!packet.isEmpty())
			{
				Serial.print(packet.pop());
				Serial.print(",");
			}
			Serial.println();			
		}
		else
		{
			EmptyQueueList(&packet);
		}
	}  
}
