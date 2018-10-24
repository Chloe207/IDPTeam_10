
#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stdlib.h>     //for using the function sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stopwatch.h>
#define ROBOT_NUM 14   // The id number (see below)
#define middle 0
#define left 1
#define back 2
#define right 3
#define white 4
#define	blue 5
#define	trigger 6

robot_link rlink;      // datatype for the robot link

int main () { 
	bool IR[7];
	int IR_data;                            // data from microprocessor
	if (!rlink.initialise (ROBOT_NUM)) { // setup the link
	  cout << "Cannot initialise link" << endl;
	  rlink.print_errs("  ");
	  return -1;
	}

	for (int t=1; t<30000; t = t+1) {
		IR_data = rlink.request(READ_PORT_0);	    			    // Read value from IR board

		for (int k = 0; k < 7; k++) {
			IR[k] = (IR_data & ( 1 << k )) >> k;
			cout << IR[k] << endl;
			
		}
	}
	return 0;
}


