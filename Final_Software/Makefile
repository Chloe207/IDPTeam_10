# Automatically generated Makefile for arm
main.arm :  package_handling.arm.o navigation.arm.o main.arm.o
	 arm-linux-gnueabi-g++ -L/usr/arm-unknown-linux-gnueabi/lib -o main.arm  package_handling.arm.o  navigation.arm.o  main.arm.o  -lrobot

package_handling.arm.o: package_handling.cc robot.h
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c package_handling.cc -o package_handling.arm.o
navigation.arm.o: navigation.cc robot.h
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c navigation.cc -o navigation.arm.o
main.arm.o: main.cc robot.h
	 arm-linux-gnueabi-g++ -ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot -c main.cc -o main.arm.o
