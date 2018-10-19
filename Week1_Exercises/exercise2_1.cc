
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
for (int t = 0; t < 1; t++) {
	robot_link rlink;      // datatype for the robot link
if (!rlink.initialise (ROBOT_NUM)) { // setup the link
  cout << "Cannot initialise link" << endl;
  rlink.print_errs("  ");
  return -1;
}
val = rlink.request (TEST_INSTRUCTION); // send test instruction
if (val == TEST_INSTRUCTION_RESULT) {   // check result
  //cout << "Test passed" << endl;
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
etime = watch.stop();
cout << etime<< endl;
return 0;                            // all OK, finish

}


