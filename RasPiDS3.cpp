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
bool DualShock3::precisionFlag = false;

DualShock3::DualShock3() {
	init("/dev/input/js0", false);
}

DualShock3::DualShock3(bool precision) {
	init("/dev/input/js0", precision);
}
DualShock3::DualShock3(const char* fileName, bool precision) {
	init(fileName, precision);
}

void DualShock3::init(const char* fileName, bool precision) {
	if (threadFlag)
		return;
	precisionFlag = precision;
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
	yReverse = false;
	loopFlag = true;
	threadFlag = true;
	readThread = thread([&]{ readLoop(); });
}

void DualShock3::precisionMode(bool precision) {
	precisionFlag = precision;
}

void DualShock3::read() {
	vector<char> data;
	char c;

	while (data.size() != 8) {
		c = JoyStick.get();
		data.push_back(c);
	}
	if (data[6] == 0x01) {
		if (data[7] >= 0 && data[7] <= NumButtons) {
			int index = data[7];
			if (data[4] == 0x00) {
				readButtonData[index] = false;
			} else if (data[4] == 0x01) {
				readButtonData[index] = true;
			}
		}
	} else if (data[6] == 0x02) {
		if (data[7] > 0x10) {
			data.clear();
			assert(data.empty());
		}
		if ((data[7] >= 0 && data[7] <= NumSticks) || data[7] == 0x0c || data[7] == 0x0d) {
			int index = data[7];
			if (data[7] == 0x0c || data[7] == 0x0d) {
				index -= 8;
			}
			readStickData[index] = data[5];
			if (readStickData[index] >= 128) 
				readStickData[index] -= 256;
			if (precisionFlag) {
				readStickData[index] *= 0x100;
				readStickData[index] += data[4];
				if (data[7] != index) 
					readStickData[index] += 32767;
			} else if (data[7] != index) 
				readStickData[index] += 128;
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

void DualShock3::yReverseSet(bool setVar) {
	yReverse = setVar;
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
	if (yReverse && (Stick == LEFT_Y || Stick == RIGHT_Y)) {
		return -stickData[Stick];
	}
	return stickData[Stick];
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
