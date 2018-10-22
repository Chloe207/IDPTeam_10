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
#include <cmath>

#define ROBOT_NUM 14   // The id number (see below)
#define middle 0
#define left 1
#define back 2
#define right 3
#define ramp_speed 255

int board_0, board_1, left_speed, right_speed, junction_no = 0, lost_no = 0;
bool IR[4], debug, junction_detected;

robot_link rlink;                      // datatype for the robot link

stopwatch watch;                       // setup watch


// Create classes
class junction {
    
    // Access Specifier
public:
    
    // Data members
    int number;         // Junction number
    int type;           // Junction type - pickup, delivery, node 0,1,2
    bool package;       // Contains package - Yes/No
    
    // Member functions()
    void print()
    {
        cout << "Junction number: " << number << endl;
        cout << "Junction type: ";
        if (type == 0) { cout << "pickup" << endl;}
        if (type == 1) { cout << "dropoff";}
        if (type == 2) { cout << "node";}
        cout << endl;
        
        if (type == 0) {
            cout << "Contains Package: ";
            if (package == 0) { cout << "No";}
            if (package == 1) { cout << "Yes";}
        }
    }
}

class package {
    
    // Access specifier
public:
    
    // Data members
    int colour;
    int delivery_location;
}
