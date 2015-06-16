#include <iostream>
#include <fstream>
#include <thread>
#include <cstring>
#include <cassert>
#include <vector>
#include "RasPiDS3.hpp"

using namespace std;
using namespace DS3;

bool RasPiDS3::threadFlag = false;
bool RasPiDS3::readButtonData[NumButtons] = {};
int RasPiDS3::readStickData[NumSticks] = {};
bool RasPiDS3::buttonData[NumButtons] = {};
int RasPiDS3::stickData[NumSticks] = {};


RasPiDS3::RasPiDS3() {
	init("/dev/input/js0");
}

RasPiDS3::RasPiDS3(const char* fileName) {
	init(fileName);
}

RasPiDS3::RasPiDS3(int sub) {
	;
}

void RasPiDS3::init(const char* fileName) {
	loopFlag = false;
	for (int i = 0; i < NumButtons; ++i) {
		readButtonData[i] = false;
		buttonData[i] = false;
	} 
	for (int i = 0; i < NumSticks; ++i) {
		readStickData[i] = false;
		stickData[i] = false;
	}
	if (threadFlag)
		return;
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

void RasPiDS3::read() {
	vector<char> data;
	char c;

	while (true) {
		c = JoyStick.get();
		cout << c << endl;
		data.push_back(c);
		if (data.size() == 8) {
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
					break;
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
			return;
		}
	}
}

void RasPiDS3::readLoop() {
	while (loopFlag) {
		read();
	}
}

void RasPiDS3::update() {
	memcpy(buttonData, readButtonData, sizeof(buttonData));
	memcpy(stickData, readStickData, sizeof(stickData));
}

bool RasPiDS3::button(ButtonsNum Button, bool onlyFlag) {
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

int RasPiDS3::stick(SticksNum Stick) {
//	return stickData[Stick];
	return readStickData[Stick];
}

RasPiDS3::~RasPiDS3() {
	loopFlag = false;
	readThread.join();
	JoyStick.close();
	threadFlag = false;
}

