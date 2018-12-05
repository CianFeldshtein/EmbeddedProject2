/**
 * \file
 *         Collector for assessment project
 * \author Cian Feldshtein
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
//If broadcast signal received executes the following code
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
	
	//nothing to execute
	
}
//Calls broadcast_recv if a broadcast is recieved, executing whats in the broadcast_recv method
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};


/*---------------------------------------------------------------------------*/

PROCESS_THREAD(broadcast_process, ev, data)
{
	
	//declaring the etimer
	static struct etimer et;

	//On process exit, close broadcast connection
 	PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

 	PROCESS_BEGIN();
	
	//Open the connection
 	broadcast_open(&broadcast, 121, &broadcast_call);

  while(1) {

			PROCESS_WAIT_EVENT();

			//Broadcast packet to all nodes
			broadcast_send(&broadcast);
		
			//Reset etimer
			etimer_reset(&et);
		
 	}

  	PROCESS_END();
}
/*---------------------------------------------------------------------------*/