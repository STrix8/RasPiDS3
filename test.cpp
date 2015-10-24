#include <iostream>
#include <unistd.h>
#include "RasPiDS3.hpp"

using namespace std;
using namespace RPDS3;

int main(void) {
	cout << "Start" << endl;
	DualShock3 Controller;
	Controller.update();
	UPDATELOOP (Controller, !Controller.button(START)) {
		for (int j = 0; j < NumButtons; ++j) {
			if (Controller.button(ButtonsNum(j)))
				cout << j << endl;
		}
		for (int j = 0; j < NumSticks; ++j) {
			cout << Controller.stick(SticksNum(j)) << " ";
		}
		cout << " ";
		for (int j = 0; j < NumAxis; ++j) {
			cout << Controller.acceleration(AxisNum(j)) << " ";
		}
		cout << endl;
		usleep(14000);
	}
	return 0;
}

