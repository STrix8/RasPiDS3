#include <iostream>
#include <fstream>
#include <thread>
#include <cstring>
#include <cassert>
#include <vector>
#include "RasPiDS3.hpp"

using namespace std;
using namespace RPDS3;

bool DualShock3::threadFlag = false;
bool DualShock3::readButtonData[NumButtons] = {};
int DualShock3::readStickData[NumSticks] = {};
bool DualShock3::buttonData[NumButtons] = {};
int DualShock3::stickData[NumSticks] = {};
bool DualShock3::beforeButtonData[NumButtons] = {};

DualShock3::DualShock3() {
	init("/dev/input/js0");
}

DualShock3::DualShock3(const char* fileName) {
	init(fileName);
}

void DualShock3::init(const char* fileName) {
	if (threadFlag)
		return;
	loopFlag = false;
	for (int i = 0; i < NumButtons; ++i) {
		readButtonData[i] = false;
		buttonData[i] = false;
	} 
	for (int i = 0; i < NumSticks; ++i) {
		readStickData[i] = false;
		stickData[i] = false;
	}
	cout << "Connect DualShock3." << endl;
	for (;;) {
		try {
			JoyStick.open(fileName);
			if (JoyStick.is_open()) {
				cout << "Connected." << endl;
				break;
			}
		}
		catch (...) {
			continue;
		}
	}
	loopFlag = true;
	threadFlag = true;
	readThread = thread([&]{ readLoop(); });
}

void DualShock3::read() {
	vector<char> data;
	char c;

	while (data.size() != 8) {
		c = JoyStick.get();
		data.push_back(c);
	}
	if (data[6] == 0x01) {
		for (int i = 0; i < NumButtons; ++i) {
			if (data[7] == i) {
				if (data[4] == 0x00) {
					readButtonData[i] = false;
				} else if (data[4] == 0x01) {
					readButtonData[i] = true;
				}
			}
		}
	} else if (data[6] == 0x02) {
		if (data[7] > 0x10) {
			data.clear();
			assert(data.empty());
		}
		for (int i = 0; i < NumSticks; ++i) {
			if (data[7] == i) {
				readStickData[i] = data[5];
				if (readStickData[i] >= 128) {
					readStickData[i] -= 256;
				}
			}
		}
	}
}

void DualShock3::readLoop() {
	while (loopFlag) {
		read();
	}
}

void DualShock3::update() {
	memcpy(beforeButtonData, buttonData, sizeof(beforeButtonData));
	memcpy(buttonData, readButtonData, sizeof(buttonData));
	memcpy(stickData, readStickData, sizeof(stickData));
}

bool DualShock3::button(ButtonsNum Button, bool onlyFlag) {
	if (onlyFlag) {
		for (int i = 0; i < NumButtons; ++i) {
			if (buttonData[i]) {
				if (i != Button)
					return false;
			}
		}
	}
	return buttonData[Button];
}

int DualShock3::stick(SticksNum Stick) {
//	return stickData[Stick];
	return readStickData[Stick];
}

DualShock3::~DualShock3() {
	loopFlag = false;
	readThread.join();
	JoyStick.close();
	threadFlag = false;
}

bool DualShock3::press(ButtonsNum Button) {
	return !beforeButtonData[Button] && buttonData[Button];
}

bool DualShock3::release(ButtonsNum Button) {
	return beforeButtonData[Button] && !buttonData[Button];
}
