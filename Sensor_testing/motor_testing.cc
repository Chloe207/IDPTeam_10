
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

val = rlink.request (TEST_INSTRUCTION); // send test instruction
if (val == TEST_INSTRUCTION_RESULT) {   // check result
rlink.command(MOTOR_1_GO, 127);
rlink.command(MOTOR_2_GO, 255);
//rlink.command(MOTOR_3_GO, 127);
//rlink.command(MOTOR_4_GO, 127);
rlink.command (RAMP_TIME, 255);			// default ramp time

delay(3000);

rlink.command(MOTOR_1_GO, 0);
rlink.command(MOTOR_2_GO, 0);
//rlink.command(MOTOR_3_GO, 0);
//rlink.command(MOTOR_4_GO, 0);
  
}
else if (val == REQUEST_ERROR) {
  cout << "Fatal errors on link:" << endl;
  rlink.print_errs();
}
else {
  cout << "Test failed (bad value returned)" << endl;
return -1;                          // error, finish
}

cout << "Test passed" << endl;
return 0;                            // all OK, finish
}


