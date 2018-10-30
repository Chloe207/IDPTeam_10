
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

stopwatch watch;                       					// setup watch

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
	watch.start();
	while (watch.read() < 3000) {
		rlink.command(MOTOR_3_GO, 110);
		delay(0.1);
	}
	while ((watch.read() < 3100) && (watch.read() > 3000))  {
		rlink.command(WRITE_PORT_1, 64+32);     // Push actuator forward whilst remaining up
		delay(0.1);
    }
    delay(2000);
    while ((watch.read() < 6100)&&(watch.read() > 3100)) {
		rlink.command(MOTOR_3_GO, 238);
		delay(0.1);
	}
	watch.stop();
    rlink.command(WRITE_PORT_1, 0);                 // Pull back while down
    delay(500);

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


