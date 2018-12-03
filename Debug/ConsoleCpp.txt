#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
//#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <windows.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#define _OPEN_SYS_ITOA_EXT




using namespace System;
using namespace System::IO::Ports;
//using namespace System::IO::Ports::SerialPort;
//using namespace std;

int main(array<System::String ^> ^args)
{

	SerialPort port("COM19", 9600);
	port.Open();
	std::ifstream myReadFile;
	//int check = 0;
	//int increase = 0;
	while (true) {
		System::Threading::Thread::Sleep(300);

		float input[3];
		myReadFile.open("C:/Fall 2018 textbook/Robotics/use/kinect-2-face-hd-master/Kinect2FaceHD/Kinect2FaceHD_NET/bin/x64/Debug/name.txt");

		if (myReadFile.is_open()) {
			//	while (!myReadFile.eof()) {

			for (int i = 0; i < 3; i++) {
				myReadFile >> input[i];


			}

			std::cout << "X is: " << input[0] << std::endl;
			std::cout << "Y is: " << input[1] << std::endl;
			std::cout << "Z is: " << input[2] << std::endl;

			//}
		}
		else {
			std::cout << "No file to open" << std::endl;
		}


		array<unsigned char>^ texBufArray = gcnew array<unsigned char>(50);

		pin_ptr<byte> p = &texBufArray[0];

		memcpy(p, &input, 12);
		port.Write(texBufArray, 0, 12);
		String^ test = port.ReadLine();					// This line wait for arduino to answer before continue
		Console::WriteLine(test);

		myReadFile.close();
		//System::Threading::Thread::Sleep(300);
	}
	return 0;
}

