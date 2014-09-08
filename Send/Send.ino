#include "cc2500_REG.h"
#include "cc2500_VAL.h"
#include "cc2500init.h"
#include "read_write.h"
#include <QueueList.h>

#include <SPI.h>
#define CC2500_RX      0x34      // Enable RX. Perform calibration if enabled
#define CC2500_TX      0x35      // Enable TX. If in RX state, only enable TX if CCA passes
#define CC2500_IDLE    0x36      // Exit RX / TX, turn
#define CC2500_FRX     0x3A      // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
#define CC2500_FTX     0x3B      // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
#define CC2500_SWOR    0x38
#define CC2500_TXFIFO  0x3F
#define CC2500_RXFIFO  0x3F
#define CC2500_SRES    0x30       // reset strobe 

#define PACKET_LENGTH 0X07
#define SEND_FORLOOP 0x08
#define ADDRESS 0xFF

QueueList <byte> myQueue;

//char TP[] = {7, 0x05, 'H','E','L','L','O','!'}; //packet length (includes address and data), device adress 
void setup(){
  
  Serial.begin(9600);  
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE0); 
  SPI.begin();
  SendStrobe(CC2500_SRES);
  init_CC2500_V2();
  
}
void loop()
{ 
  myQueue.push(0x2);
  myQueue.push(0xFF);
  myQueue.push(0x12);  
  sendPacket(&myQueue,2); 
} 

