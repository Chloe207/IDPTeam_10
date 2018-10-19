#include <iostream>
using namespace std;
#include <robot_instr.h>
#include <robot_link.h>
#include <stdlib.h>     //for using the function sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#define ROBOT_NUM 14   // The id number (see below)
robot_link rlink;      // datatype for the robot link

int main ()
{
int val;                              // data from microprocessor
if (!rlink.initialise (ROBOT_NUM)) { // setup the link
  cout << "Cannot initialise link" << endl;
  rlink.print_errs("  ");
  return -1;
}
val = rlink.request (TEST_INSTRUCTION); // send test instruction
if (val == TEST_INSTRUCTION_RESULT) {   // check result
  rlink.command(WRITE_PORT_3, 1);
  sleep(1);
  rlink.command(WRITE_PORT_3, 1+2);
  sleep(1); 
  rlink.command(WRITE_PORT_3, 1+2+4);
  sleep(1); 
  rlink.command(WRITE_PORT_3, 1+2+4+8);
  sleep(1); 
  rlink.command(WRITE_PORT_3, 1+2+4+8+16);
  sleep(1); 
  rlink.command(WRITE_PORT_3, 1+2+4+8+16+32);
  sleep(5); 
  rlink.command(WRITE_PORT_3, 1+2+4+8+16+32+64);
  sleep(5); 
  rlink.command(WRITE_PORT_3, 1+2+4+8+16+32+64+128);
  int v=rlink.request (READ_PORT_3);
  cout << "Value="  <<v << endl;
  cout << "Test passed" << endl;
  return 0;                            // all OK, finish
}
else if (val == REQUEST_ERROR) {
  cout << "Fatal errors on link:" << endl;
  rlink.print_errs();
}
else
  cout << "Test failed (bad value returned)" << endl;
return -1;                          // error, finish
}
