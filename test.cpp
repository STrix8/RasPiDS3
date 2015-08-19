#include <iostream>
#include "RasPiDS3.hpp"

using namespace std;
using namespace RPDS3;

int main(void) {
	cout << "Start" << endl;
	DualShock3 Controller;
	Controller.update();
	for (int i = 2048; --i; Controller.update()) {
		cout << i << " ";
		for (int j = 0; j < NumButtons; ++j) {
			if (Controller.button(ButtonsNum(j)))
				cout << j << endl;
		}
		for (int j = 0; j < NumSticks; ++j) {
			cout << Controller.stick(SticksNum(j)) << " ";
		}
		cout << endl;
	}
	return 0;
}

