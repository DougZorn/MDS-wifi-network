#include "cc2500_REG.h"
#include "cc2500_VAL.h"
#include "cc2500init.h"
#include "read_write.h"
#include <SPI.h>
#include <QueueList.h>
#include "node_config.h"

QueueList <byte> myQueue1;
QueueList <byte> myQueue2;

void setup()
{
	Serial.begin(9600);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setDataMode(SPI_MODE0);  
	SPI.begin();  
	SendStrobe(CC2500_SRES);
	init_CC2500_V2();
	pinMode(9, OUTPUT);
	digitalWrite(9, LOW);
	
	/*
	Serial.print("Node: ");
	Serial.println(NODE_ID_ADDRESS, HEX);
	Serial.print("Initial state: ");
	Serial.println(INITIAL_STATE, HEX);
	*/	
} 

#define Bpacket 0x07
#define NB1packet 0x06
#define NB2packet 0x06
#define NTDMpacket 0x05

//length (not including self), address, type, Self ID, packet type. The length byte shows up on RX FIFO
byte BroadcastPacket[] = {0x04,0xFF,0x01,NODE_ID_ADDRESS,Bpacket}; //bytes on RXFIFO: 7
byte HandShakeNextBroadcast[] = {0x03,NEXT_NODE_ADDRESS,NB1packet,NB2packet}; //bytes on RXFIFO: 6
byte HandShakeNextTransmitDM[] = {0x02,NEXT_NODE_ADDRESS,NTDMpacket}; //bytes on RXFIFO: 5

//states FIX NUMBERS !!!! DUPLICATES
#define SEND_HANDSHAKE_PACKET_B 0x01
#define SEND_HANDSHAKE_PACKET_DM 0x02
#define WAIT_4_ACK 0x03
#define LISTEN_4_PACKET 0x04
#define BROADCAST 0x05

byte state = INITIAL_STATE;
//byte packet[] = {0x01, 0xFF};

void loop()
{
	switch(state)
	{
		case LISTEN_4_PACKET:
			Serial.println("Entered LISTEN_4_PACKET STATE");
			if(listenForPacket(&myQueue1))	//edit return type to be one of the three posssible values
			{
				Serial.println("packet Received...");
				Serial.print("myQueue count: ");
                Serial.println(myQueue1.count(),HEX);				
				if(Bpacket == myQueue1.count()) //expect peek to be 1 less then count. peek is the lenght which does not include itself
				{
					Serial.println("Storing data...");
					state = LISTEN_4_PACKET; //you need to write code to store the data
				}
				else if(NB1packet == myQueue1.count())
				{
					Serial.println("Broadcast HandShake Recieved...");
					state = BROADCAST;
				}
				else //5
				{
					Serial.println("DisM HandShake Received");
					Serial.println("Sending Dis Matrix...");
					//insert code to transmit dis matrix to processing node
					if (LAST_NODE == NODE)
					{			   
						state = SEND_HANDSHAKE_PACKET_B;
					}
					else
					{
						state = SEND_HANDSHAKE_PACKET_DM;
					}
				
				}				
				EmptyQueueList(&myQueue1); //this is just to test state transitions	
			}
			break;
		case BROADCAST:
			Serial.println("Entered BROADCAST STATE");
			digitalWrite(9, HIGH);
			//delay(1000);
			sendFixedPacket(BroadcastPacket,5);			
			if (LAST_NODE == NODE)
			{
				//Sending DisMatrix
				state = SEND_HANDSHAKE_PACKET_DM;
			}
			else
			{				
				state = SEND_HANDSHAKE_PACKET_B;
			}			
			break;
		case SEND_HANDSHAKE_PACKET_B:
			Serial.println("Entered SEND_HANDSHAKE_B STATE");
			sendFixedPacket(HandShakeNextBroadcast,4);						
			state = WAIT_4_ACK;
			break;
		case SEND_HANDSHAKE_PACKET_DM:                  // this currently does not have an ACK when transmitting the DM
			Serial.println("Entered SEND_HANDSHAKE_PACKET_DM STATE");			
			sendFixedPacket(HandShakeNextTransmitDM,3);
			digitalWrite(9, LOW);
			state = LISTEN_4_PACKET;
			break;
		case WAIT_4_ACK:
			Serial.println("Entered WAIT_4_ACK STATE");
			if(listenForPacket(&myQueue2)) //listenForPacket is blocking function that has timeout built in as return value 0
			{
				Serial.println("ACK received");
				state = LISTEN_4_PACKET;
				digitalWrite(9, LOW);
				EmptyQueueList(&myQueue2); //this is just to test state transitions	
			}
			else
			{
				state = SEND_HANDSHAKE_PACKET_B;
			}			
			break;		
	}
} 


