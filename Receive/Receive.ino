#include "cc2500_REG.h"
#include "cc2500_VAL.h"
#include "cc2500init.h"
#include "read_write.h"
#include <SPI.h>
#include <QueueList.h>
#include "node_config.h"

QueueList <byte> myQueue1;
QueueList <byte> myQueue2;
QueueList <byte> DMpacket;

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
} 

#define ACK_DM 0x08
#define Bpacket 0x07
#define NB1packet 0x06
#define NTDMpacket 0x05

//length (not including self), address, type, Self ID, packet type. The length byte shows up on RX FIFO
byte ACKDMPACKET[] = {0x05,0xFF,0x01,NODE_ID_ADDRESS,ACK_DM,ACK_DM}; //bytes on RXFIFO: 8
byte BroadcastPacket[] = {0x04,0xFF,0x01,NODE_ID_ADDRESS,Bpacket}; //bytes on RXFIFO: 7
byte HandShakeNextBroadcast[] = {0x03,NEXT_NODE_ADDRESS,NB1packet,NB1packet}; //bytes on RXFIFO: 6
byte HandShakeNextTransmitDM[] = {0x02,NEXT_NODE_ADDRESS,NTDMpacket}; //bytes on RXFIFO: 5

//states FIX NUMBERS !!!! DUPLICATES
#define SEND_HANDSHAKE_PACKET_B 0x01
#define SEND_HANDSHAKE_PACKET_DM 0x02
#define WAIT_4_ACK_B 0x03
#define LISTEN_4_PACKET 0x04
#define BROADCAST 0x05
#define WAIT_4_ACK_DM 0x06
#define SEND_ACK_DM 0x07// did not implement

byte state = INITIAL_STATE;
//byte packet[] = {0x01, 0xFF};

byte Dmatrix[LAST_NODE] = {0};
byte NodeID = 0;
byte RSSI = 0;

void loop()
{
	switch(state)
	{
		case LISTEN_4_PACKET:
			#if debug
			Serial.println("Entered LISTEN_4_PACKET STATE");
			#endif
			if(listenForPacket(&myQueue1))	//edit return type to be one of the three posssible values
			{
				#if debug
				Serial.println("packet Received...");	
				#endif
				if(Bpacket == myQueue1.count()) //expect peek to be 1 less then count. peek is the lenght which does not include itself
				{
					NodeID = 0;
					#if debug
					Serial.println("Storing data...");
					#endif
					myQueue1.pop();
					myQueue1.pop();
					myQueue1.pop();
					NodeID = myQueue1.pop()-1;
					myQueue1.pop();					
					Dmatrix[NodeID] = myQueue1.pop();
					myQueue1.pop();
					//Dmatrix[NodeID] = convert_dBm(RSSI); //lets convert at the other end
					//Serial.println(Dmatrix[NodeID],DEC);
					state = LISTEN_4_PACKET; //you need to write code to store the data
				}
				else if(NB1packet == myQueue1.count())
				{
					#if debug
					Serial.println("Broadcast HandShake Recieved...");
					#endif
					state = BROADCAST;
				}
				else if(NTDMpacket == myQueue1.count())//5
				{
					#if debug
					Serial.println("DisM HandShake Received");
					Serial.println("Sending Dis Matrix...");
					#endif
					//sendFixedPacket(ACKDMPACKET,6);			//!!!!!!!!!!!!!!!
					SendDMmatrix(Dmatrix,&DMpacket);
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
				else
				{
					state = LISTEN_4_PACKET;
				}
				EmptyQueueList(&myQueue1); //this is just to test state transitions	
			}
			break;
			
		case SEND_ACK_DM:
			
		
			break;
		case BROADCAST:
			#if debug
			Serial.println("Entered BROADCAST STATE");
			#endif
			digitalWrite(9, HIGH);			
			sendFixedPacket(BroadcastPacket,5);			
			if (LAST_NODE == NODE)
			{
				//Sending DisMatrix
				#if debug
				Serial.println("DisM HandShake Received");
				Serial.println("Sending Dis Matrix...");
				#endif
				//sendFixedPacket(ACKDMPACKET,6); //!!!!!!!!!!!!!!!
				SendDMmatrix(Dmatrix,&DMpacket);
				state = SEND_HANDSHAKE_PACKET_DM;
			}
			else
			{				
				state = SEND_HANDSHAKE_PACKET_B;
			}			
			break;
		case SEND_HANDSHAKE_PACKET_B:
			#if debug
			Serial.println("Entered SEND_HANDSHAKE_B STATE");
			Serial.print("Sent to: ");
			Serial.println(NEXT_NODE_ADDRESS,HEX);
			#endif
			sendFixedPacket(HandShakeNextBroadcast,4);						
			state = WAIT_4_ACK_B;
			break;
		case SEND_HANDSHAKE_PACKET_DM:                  // this currently does not have an ACK when transmitting the DM
			#if debug
			Serial.println("Entered SEND_HANDSHAKE_PACKET_DM STATE");
			Serial.print("Sent to: ");
			Serial.println(NEXT_NODE_ADDRESS,HEX);
			#endif
			sendFixedPacket(HandShakeNextTransmitDM,3);
			digitalWrite(9, LOW);
			state = WAIT_4_ACK_DM;
			break;
		case WAIT_4_ACK_DM:
			#if debug
			Serial.println("Entered WAIT_4_ACK_DM STATE");
			#endif
			if(listenForPacket(&myQueue2)) //listenForPacket is blocking function that has timeout built in as return value 0
			{	
				#if debug
				Serial.println("ACK_DM received");				
				#endif
				state = LISTEN_4_PACKET;
				digitalWrite(9, LOW);
				EmptyQueueList(&myQueue2); //this is just to test state transitions	
			}
			else
			{
				state = SEND_HANDSHAKE_PACKET_DM;
			}			
			break;			
		case WAIT_4_ACK_B:
			#if debug
			Serial.println("Entered WAIT_4_ACK_B STATE");
			#endif
			if(listenForPacket(&myQueue2)) //listenForPacket is blocking function that has timeout built in as return value 0
			{	
				NodeID = 0;
				#if debug
				Serial.println("ACK_B received");
				Serial.println("Storing data...");
				#endif
				myQueue2.pop();
				myQueue2.pop();
				myQueue2.pop();
				NodeID = myQueue2.pop()-1;				
				myQueue2.pop();
				Dmatrix[NodeID] = myQueue2.pop();
				myQueue2.pop();
				// Dmatrix[NodeID] = convert_dBm(RSSI);
				#if debug
				Serial.println(Dmatrix[NodeID],DEC);	
				#endif
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


