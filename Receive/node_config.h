#ifndef NODE_CONFIG_H
#define NODE_CONFIG_H

#define TX_TIMEOUT 150 // in milliseconds
#define LAST_NODE 0x02 

#define NODE 0x02 //set for each node. Settings for each are below

//when defining NEXT_NODE ADDRESS
// states
// SEND_HANDSHAKE_PACKET_B 0x01
// SEND_HANDSHAKE_PACKET_DM 0x02
// WAIT_4_ACK 0x03
// LISTEN_4_PACKET 0x04
// BROADCAST 0x05
// WAIT_4_ACK_DM 0x06

#if NODE == 0x01
	#define NODE_ID_ADDRESS 0x01
	#define NEXT_NODE_ADDRESS 0x02
	#define INITIAL_STATE 0x04
#elif NODE == 0x02
	#define NODE_ID_ADDRESS 0x02
	#define NEXT_NODE_ADDRESS 0x01
	#define INITIAL_STATE 0x05
#elif NODE == 0x03
	#define NODE_ID_ADDRESS 0x03
	#define NEXT_NODE_ADDRESS 0x04
	#define INITIAL_STATE 0x04
#elif NODE == 0x04
	#define NODE_ID_ADDRESS 0x04
	#define NEXT_NODE_ADDRESS 0x05
	#define INITIAL_STATE 0x04
#elif NODE == 0x05
	#define NODE_ID_ADDRESS 0x05
	#define NEXT_NODE_ADDRESS 0x01
	#define INITIAL_STATE 0x05
#endif

#endif
