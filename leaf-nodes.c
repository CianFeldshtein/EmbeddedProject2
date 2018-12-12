/**
 * \file
 *        author Cian Feldshtein
 *        cian.feldshtein@mycit.ie
 *        R00142270 
 */

#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>
/*---------------------------------------------------------------------------*/

//Declare the process
PROCESS(example_unicast_process, "leaf-nodes");

//Make the process start 
AUTOSTART_PROCESSES(&example_unicast_process);

/*---------------------------------------------------------------------------*/

//Unicast connection
static struct unicast_conn uc;
//Broadcast connection
static struct broadcast_conn broadcast;

static int hopCount = 0;
static int sequenceNum = 0;
static linkaddr_t *parent;
static int connected = 0;

//Struct containing the readings from the nodes. This will be sent to the source
struct beacon_Message{
	
	int hopCount;
	int sequenceNum;

};


/*---------------------------------------------------------------------------*/

//If broadcast signal received executes the following code
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
	//creating beacon the pointer msg
	struct beacon_Message *message;
    //receiving values
	message = packetbuf_dataptr();

	//reading the values sent to us
	int newHopCount = message->hopCount;
	int newSequenceNum = message->sequenceNum;
	//if its a new beacon broadcast
	if(sequenceNum < newSequenceNum){ 
		
		hopCount = newHopCount + 1;
		sequenceNum = newSequenceNum;
        //copy address from parent
		linkaddr_copy(parent, from);

		printf("The new beacon parent is: %d.%d\n\n", from->u8[0], from->u8[1]);

		connected = 1;

        //message points to the start memory which is in the packet buffer and we tell it it is a struct beacon
		message = (struct beacon_Message *) packetbuf_dataptr(); 
		packetbuf_set_datalen(sizeof(struct beacon_Message));

		message->hopCount = hopCount;
		message->sequenceNum= sequenceNum;

		broadcast_send(&broadcast);

	}
    //if new hop count is better than previous
	else if(hopCount > newHopCount +1){

			hopCount = newHopCount + 1;
			linkaddr_copy(parent, from);

			printf("New parent path back to source based on hop count is through parent: %d.%d\n\n", from->u8[0], from->u8[1]);

			connected = 1;

			packetbuf_clear();

			message = (struct beacon_Message *) packetbuf_dataptr(); 
			packetbuf_set_datalen(sizeof(struct beacon_Message));

			message->hopCount = hopCount;
			message->sequenceNum= sequenceNum;

			broadcast_send(&broadcast);

	}
	
}
//Calls broadcast_recv if a broadcast is received, executing whats in the broadcast_recv method
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};

/*---------------------------------------------------------------------------*/

//Define the process code
PROCESS_THREAD(non_root_process, ev, data)
{
	
	// declaring etimer
	static struct etimer et;
	
	//On process exit, close both broadcast and unicast connections
	PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
	PROCESS_EXITHANDLER(unicast_close(&uc);)

	PROCESS_BEGIN();
	
	//Open connections
	broadcast_open(&broadcast, 121, &broadcast_call);

	while(1){
		//set to 7 seconds
                etimer_set(&et, CLOCK_SECOND * 7);
		PROCESS_WAIT_EVENT();
                        
			if(connected == 0){
			
				hopCount = -1;

				printf("NOT CONNECTED\n\n");

			}
                        connected = 0;
	}

	PROCESS_END();
}

/*---------------------------------------------------------------------------*/