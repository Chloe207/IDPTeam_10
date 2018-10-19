// Write a function called set_i_to_7 in another file so
// that when the other file and this file are compiled
// together, the resulting program prints "i=7".
// Don't change this file

#include <iostream>
using namespace std;

void set_i_to_7();

int i=0;

int main() {
  set_i_to_7();
  cout << "i=" << i << endl;
}
