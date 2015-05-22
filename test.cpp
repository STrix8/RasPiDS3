#include <iostream>
#include "RasPiDS3.hpp"

using namespace std;
using namespace DS3;

int main(void) {
	cout << "Start" << endl;
	RasPiDS3 Controller;
	Controller.update();
	for (int i = 4096; --i; Controller.update()) {
		for (int j = 0; j < NumButtons; ++j) {
			if (Controller.button((SticksNum)j))
				cout << j << endl;
		}
		for (int j = 0; j < NumSticks; ++j) {
			cout << Controller.stick(j) << " ";
		}
		cout << endl;
	}
	return 0;
}

