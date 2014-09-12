#ifndef NODE_CONFIG_H
#define NODE_CONFIG_H
#define debug 0x01

//define node # when programming individual nodes
#define NODE 0x02
//define number of nodes in network
#define NUMBER_OF_NODES 0x02
//Packet timeout in milliseconds
#define TX_TIMEOUT 150 
#define LAST_NODE NUMBER_OF_NODES


// STATES
// SEND_HANDSHAKE_PACKET_B 0x01
// SEND_HANDSHAKE_PACKET_DM 0x02
// WAIT_4_ACK 0x03
// LISTEN_4_PACKET 0x04
// BROADCAST 0x05
// WAIT_4_ACK_DM 0x06
// SEND_ACK_DM 0x07 // did not implement

	#if NUMBER_OF_NODES == 0x02
		#if NODE == 0x01
			#define NODE_ID_ADDRESS 0x01
			#define NEXT_NODE_ADDRESS 0x02
			#define INITIAL_STATE 0x04
		#elif NODE == 0x02
			#define NODE_ID_ADDRESS 0x02
			#define NEXT_NODE_ADDRESS 0x01
			#define INITIAL_STATE 0x05	
		#endif
	#elif NUMBER_OF_NODES == 0x03
		#if NODE == 0x01
			#define NODE_ID_ADDRESS 0x01
			#define NEXT_NODE_ADDRESS 0x02
			#define INITIAL_STATE 0x04
		#elif NODE == 0x02
			#define NODE_ID_ADDRESS 0x02
			#define NEXT_NODE_ADDRESS 0x03
			#define INITIAL_STATE 0x04		
		#elif NODE == 0x03
			#define NODE_ID_ADDRESS 0x03
			#define NEXT_NODE_ADDRESS 0x01
			#define INITIAL_STATE 0x05
		#endif	
	#elif NUMBER_OF_NODES == 0x04
		#if NODE == 0x01
			#define NODE_ID_ADDRESS 0x01
			#define NEXT_NODE_ADDRESS 0x02
			#define INITIAL_STATE 0x04
		#elif NODE == 0x02
			#define NODE_ID_ADDRESS 0x02
			#define NEXT_NODE_ADDRESS 0x03
			#define INITIAL_STATE 0x04		
		#elif NODE == 0x03
			#define NODE_ID_ADDRESS 0x03
			#define NEXT_NODE_ADDRESS 0x04
			#define INITIAL_STATE 0x04
		#elif NODE == 0x04
			#define NODE_ID_ADDRESS 0x04
			#define NEXT_NODE_ADDRESS 0x01
			#define INITIAL_STATE 0x05		
		#endif	
	#elif NUMBER_OF_NODES == 0x05
		#if NODE == 0x01
			#define NODE_ID_ADDRESS 0x01
			#define NEXT_NODE_ADDRESS 0x02
			#define INITIAL_STATE 0x04
		#elif NODE == 0x02
			#define NODE_ID_ADDRESS 0x02
			#define NEXT_NODE_ADDRESS 0x03
			#define INITIAL_STATE 0x04		
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
	#endif //number of nodes
#endif //ifndef
