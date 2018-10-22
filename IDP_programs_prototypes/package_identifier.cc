#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>
#include <stdlib.h>     //for using the function sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include <cmath>

#define ROBOT_NUM 14   // The id number (see below)

using namespace std;

int main () {
	robot_link rlink; 

	if (!rlink.initialise (ROBOT_NUM)) { 						// Setup the link
			cout << "Cannot initialise link" << endl;
			rlink.print_errs("  ");
			return -1;
		}
		else {
			cout << "Initialised"<< endl;
		}
}
	
	
