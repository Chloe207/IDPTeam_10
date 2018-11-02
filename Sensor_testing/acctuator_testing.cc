
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
stopwatch watch8;

int main () {
	robot_link rlink;      // datatype for the robot link
if (!rlink.initialise (ROBOT_NUM)) { // setup the link
  cout << "Cannot initialise link" << endl;
  rlink.print_errs("  ");
  return -1;
}
    watch8.start();
    while (watch8.read() < 2500) {
        rlink.command(MOTOR_3_GO, 110);
        delay(0.1);
    }
    while ((watch8.read() < 2700) && (watch8.read() > 2500))  {
        cout << "Arm extended" << endl;
        rlink.command(WRITE_PORT_1, 64+32);     // Push actuator forward whilst remaining up
        delay(0.1);
    }
    delay(500);
    while ((watch8.read() < 5400)&&(watch8.read() > 2700)) {
        rlink.command(MOTOR_3_GO, 238);
        delay(0.1);
    }
    watch8.stop();
    rlink.command(WRITE_PORT_1, 0);                 // Pull back while down
    delay(500);
    
    cout << "Picked up package" << endl;
    delay(500);
    rlink.command(WRITE_PORT_1, 32+64);     // Push actuator forward whilst remaining up               
    cout << rlink.request(READ_PORT_1) << endl;
    delay(500);
    
    cout << "Picked up package" << endl;
}



