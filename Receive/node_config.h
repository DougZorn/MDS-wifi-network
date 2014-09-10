#ifndef NODE_CONFIG_H
#define NODE_CONFIG_H

#define TX_TIMEOUT 100 // in milliseconds
#define LAST_NODE 0x09 

#define NODE 0x01 //set for each node. Settings for each are below

#if NODE == 0x01
	#define NODE_ID_ADDRESS 0x01
	#define NEXT_NODE_ADDRESS 0x02
	#define INITIAL_STATE 0x01
#elif NODE == 0x02
	#define NODE_ID_ADDRESS 0x02
	#define NEXT_NODE_ADDRESS 0x03
	#define INITIAL_STATE 0x01
#elif NODE == 0x03
	#define NODE_ID_ADDRESS 0x03
	#define NEXT_NODE_ADDRESS 0x04
	#define INITIAL_STATE 0x01
#elif NODE == 0x04
	#define NODE_ID_ADDRESS 0x04
	#define NEXT_NODE_ADDRESS 0x05
	#define INITIAL_STATE 0x01
#elif NODE == 0x05
	#define NODE_ID_ADDRESS 0x05
	#define NEXT_NODE_ADDRESS 0x01
	#define INITIAL_STATE 0x01
#endif

#endif