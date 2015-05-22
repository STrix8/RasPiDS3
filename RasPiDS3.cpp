#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <cstdlib>
#include "RasPiDS3.hpp"

using namespace std;
using namespace DS3;

RasPiDS3::RasPiDS3() {
	RasPiDS3("/dev/input/js0");
}
RasPiDS3::RasPiDS3(string filename) {
	loopFlag = false;
	threadFlag = false;
	for (int i = 0; i < ButtonsNum; ++i) {
		readButtonData[i] = false;
		buttonData[i] = false;
	} 
	for (int i = 0; i < SticksNum; ++i) {
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
		catch {
			continue;
		}
	}
	loopFlag = true;
	readThread = thread(readLoop);
	threadFlag = true;
}

void RasPiDS3::read() {
	vector<char> data;
	char c;
	for (int i = NumButtons; i--; readButtonData[i] = false);
	for (int i = NumSticks; i--; readStickData[i] = 0);
	while (true) {
		JoyStick.get(c);
		data.push_back(c);
		if (data.size() == 8) {
			if (data[6] == 0x01) {
				for (int i = 0; i < ButtonsNum; ++i) {
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
				for (int i = 0; i < SticksNum; ++i) {
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

static bool RasPiDS3::button(ButtonsNum Button, bool onlyFlag) {
	if (only) {
		for (int i = 0; i < ButtonsNum; ++i) {
			if (buttonData[i]) {
				if (i != Button)
					return false;
			}
		}
	}
	return buttonData[Button];
}

static int RasPiDS3::stick(SticksNum Stick) {
	return stickData[Stick];
}

RasPiDS3::~RasPiDS3() {
	loopFlag = false;
	readThread.join();
	JoyStick.close();
	threadFlag = false;
}
