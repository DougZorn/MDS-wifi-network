#include <SPI.h>
#include "cc2500_VAL.h"
#include "cc2500_REG.h"
#include <math.h>
#include <QueueList.h>
#include "node_config.h"

#ifndef READ_WRITE_H
#define READ_WRITE_H

#define CC2500_RX      0x34      // Enable RX. Perform calibration if enabled
#define CC2500_TX      0x35      // Enable TX. If in RX state, only enable TX if CCA passes
#define CC2500_IDLE    0x36      // Exit RX / TX, turn
#define CC2500_FRX     0x3A      // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
#define CC2500_FTX     0x3B      // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
#define CC2500_SWOR    0x38
#define CC2500_TXFIFO  0x3F
#define CC2500_RXFIFO  0x3F
#define CC2500_TXFIFO_BURST  0x7F
#define CC2500_RXFIFO_BURST  0xFF
#define CC2500_SRES    0x30       // reset strobe 

long previousTXTimeoutMillis = 0;

void WriteReg(char addr, char value) //see page 22 of cc2500 data sheet for timing
{
  digitalWrite(10,LOW);
  while (digitalRead(MISO) == HIGH)
  {
  };    
  SPI.transfer(addr);
  SPI.transfer(value);
  digitalWrite(10,HIGH);
}

char ReadReg(char addr){
  addr = addr + 0x80;
  digitalWrite(10,LOW);
  while (digitalRead(MISO) == HIGH) {
    };
  char x = SPI.transfer(addr);
  char y = SPI.transfer(0);
  digitalWrite(10,HIGH);
  return y;  
}

char ReadOnly_Reg(char addr){
  addr = addr + 0xC0;
  digitalWrite(10,LOW);
  while (digitalRead(MISO) == HIGH) {
    };
  char x = SPI.transfer(addr);
  char y = SPI.transfer(0);
  digitalWrite(10,HIGH);
  return y;  
}

void SendStrobe(char strobe){
  digitalWrite(10,LOW);  
  while (digitalRead(MISO) == HIGH) {
  };  
  SPI.transfer(strobe);
  digitalWrite(10,HIGH); 
}

double convert_dBm(byte RSSI)
{
	byte dBm = 0;	
	if (RSSI >= 128)
	{
		dBm = ((RSSI - 256)/2 - 71);
	}
	else
	{
		dBm = (RSSI/2 - 71);
	}	
	return dBm;
}

double convert_distance(double dBm)
{
	double distance = 0;
	distance = pow(10, log(log10(-1*dBm)/1.5270)/0.1584);
	return distance;
}

int listenForPacket(QueueList<byte> *list)
{
	int PacketType = 0;
	while(!(list->isEmpty()))
	{
		list->pop();
	}	
	previousTXTimeoutMillis = millis();
	SendStrobe(CC2500_RX);
	while(!digitalRead(MISO))   
	{
		if(millis()-previousTXTimeoutMillis > TX_TIMEOUT){
		return 0; 
		}
	}
	while(digitalRead(MISO)) 
	{
		if(millis()-previousTXTimeoutMillis > TX_TIMEOUT){
		return 0; 
		}
	}
	if(ReadOnly_Reg(0x3B)==0) //status register that returns number of bytes in RX FIFO
	{    
		return 0;    
	}
	list->push(ReadReg(CC2500_RXFIFO)+2);	
	int loopCount = list->peek(); //+2 to loopCount due to append of RSSI and LQI
	
	for(int i = 0; i <loopCount; i++)
	{
		list->push(ReadReg(CC2500_RXFIFO));
	}
	
	SendStrobe(CC2500_IDLE);  
	SendStrobe(CC2500_FRX);
	return 1;	
}

void WriteFixedTX_burst(char addr, byte value[], byte count)
{  
  addr = addr + 0x40;  
  digitalWrite(10,LOW);
  
  while (digitalRead(MISO) == HIGH) {
  };
  SPI.transfer(addr);

  for(byte i = 0; i<count; i++)
  {
    SPI.transfer(value[i]);
  }
  digitalWrite(10,HIGH);
}

void sendFixedPacket(byte packet[], byte size)
{  
  SendStrobe(CC2500_IDLE); 
  SendStrobe(CC2500_FTX);
  WriteFixedTX_burst(CC2500_TXFIFO,packet,size);
  //do not add code between the strobe and while loops otherwise it will miss the conditions !!!!!!!!!!!!!!
  SendStrobe(CC2500_TX); 
  while (!digitalRead(MISO)) { }    
  while (digitalRead(MISO)) { }    
  //do not add code between the strobe and while loops otherwise it will miss the conditions !!!!!!!!!!!!!!
  SendStrobe(CC2500_IDLE); 
}
void WriteDynamicTX_burst(char addr, QueueList<byte> *list, byte count)
{  
  addr = addr + 0x40;
  digitalWrite(10,LOW);  
  while (digitalRead(MISO) == HIGH) {
  };
  SPI.transfer(addr); 
  for(byte i = 0; i<count; i++)
  {
    SPI.transfer(list->pop());
  }
  digitalWrite(10,HIGH);  
}

void sendDynamicLengthPacket(QueueList<byte> *list, byte forloop)
{	
	SendStrobe(CC2500_IDLE); 
	SendStrobe(CC2500_FTX);	
	WriteDynamicTX_burst(CC2500_TXFIFO,list,forloop);		
	SendStrobe(CC2500_TX); 
	while (!digitalRead(MISO)) { }    
	while (digitalRead(MISO)) { }    
	//do not add code between the strobe and while loops otherwise it will miss the conditions !!!!!!!!!!!!!!
	SendStrobe(CC2500_IDLE); 
}

void EmptyQueueList(QueueList<byte> *list)
{
	while(!list->isEmpty())
	{
		list->pop();
	}
}

void SendDMmatrix(byte matrix[],QueueList<byte> *packet)
{
	byte forloop = 4 + NUMBER_OF_NODES;
	packet->push(3+NUMBER_OF_NODES);// insert length
	packet->push(0xFF); //destination address
	packet->push(0x00); //filler
	packet->push(NODE_ID_ADDRESS);
	for (int i = 0; i < NUMBER_OF_NODES; i++) // with 5 nodes this is currently between 2 and 5 inclusive
	{
		packet->push(matrix[i]);
		//Serial.print("Push number: ");
		//Serial.println(i);		
	}
	Serial.print("Total Size of DM packet (bytes) ");
	Serial.println(packet->count());
	sendDynamicLengthPacket(packet, forloop);
}


#endif
