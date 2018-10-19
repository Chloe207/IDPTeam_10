
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
stopwatch watch;


int main () {
	
double etime;
int val;                              // data from microprocessor
watch.start();
robot_link rlink;      // datatype for the robot link
if (!rlink.initialise (ROBOT_NUM)) { // setup the link
  cout << "Cannot initialise link" << endl;
  rlink.print_errs("  ");
  return -1;
}
val = rlink.request (TEST_INSTRUCTION); // send test instruction
if (val == TEST_INSTRUCTION_RESULT) {   // check result
  //rlink.command(WRITE_PORT_3, 3);
  cout << "Reading port now" << endl;
  int v=rlink.request (READ_PORT_3);
  cout << "Using the ADC" << endl;
  v = rlink.request (ADC3);
  cout << "Value="  <<v << endl;
  cout << "Test passed" << endl;
  etime = watch.stop();
  cout << etime << endl;
  return 0;                            // all OK, finish
}
else if (val == REQUEST_ERROR) {
  cout << "Fatal errors on link:" << endl;
  rlink.print_errs();
}
else {
  cout << "Test failed (bad value returned)" << endl;
return -1;                          // error, finish
}

}


