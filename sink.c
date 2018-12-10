/**
 * \
 *       
 *         author Cian Feldshtein
 *         cian.feldshtein@mycit.ie
 *         R00142270
 */

#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/

//Declare the process
PROCESS(broadcast_process, "Broadcast");

//Make the process start when the module is loaded
AUTOSTART_PROCESSES(&broadcast_process);

/*---------------------------------------------------------------------------*/
//Connections
static struct broadcast_conn broadcast;
static struct unicast_conn uc;


static int hopCount = 0;
static int sequenceNum = 0;

static struct beacon_Message{

	int hopCount; 
	int sequenceNum;

};

/*---------------------------------------------------------------------------*/
//If broadcast signal received executes the following code
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
	
	//nothing to execute here
	
}
//Calls broadcast_recv if a broadcast is recieved, executing whats in the broadcast_recv method
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};


/*---------------------------------------------------------------------------*/

PROCESS_THREAD(broadcast_process, ev, data)
{
	
	// declaring etimer
	static struct etimer et;

	//On process exit, close broadcast connection
 	PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

 	PROCESS_BEGIN();
	
	//Open connection
 	broadcast_open(&broadcast, 121, &broadcast_call);

  while(1) {
			
    	                //set to 5 seconds
                        etimer_set(&et, CLOCK_SECOND * 5);

			PROCESS_WAIT_EVENT();
			//creating beacon_Message the pointer message		
			struct beacon_Message *message;
			// message points to the start memory which is in the packet buffer and we tell it it is a struct beacon
                        //starts at the start address of beacon
			message = (struct beacon_Message *) packetbuf_dataptr(); 
                        //Set the length of the data in the packetbuf to size of beacon.
			packetbuf_set_datalen(sizeof(struct beacon_Message));
                        //copy hopCount into beacon hopCount
                        message->hopCount = hopCount;
                        sequenceNum = sequenceNum + 1;
                        message->sequenceNum = sequenceNum;

			//Broadcast packet to all sensors
			broadcast_send(&broadcast);
		
			//Reset etimer
			etimer_reset(&et);
		
 	}

  	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
