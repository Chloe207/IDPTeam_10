
#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stdlib.h>     //for using the function sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stopwatch.h>
#include <robot_delay.h>
#define ROBOT_NUM 14   // The id number (see below)


int main () {
int val;                              // data from microprocessor
	robot_link rlink;      // datatype for the robot link
if (!rlink.initialise (ROBOT_NUM)) { // setup the link
  cout << "Cannot initialise link" << endl;
  rlink.print_errs("  ");
  return -1;
}

    rlink.command(WRITE_PORT_1, 32+64);     // Push actuator forward whilst remaining up
    //delay(500);
    //rlink.command(WRITE_PORT_1, 0);                 // Pull back while down
    cout << rlink.request(READ_PORT_1) << endl;
    delay(500);
    
    cout << "Picked up package" << endl;
}



