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
			if (Controller.button(j))
				cout << j << endl;
		}
		for (int j = 0; j < StickNums; ++j) {
			cout << Controller.stick(j) << " ";
		}
		cout << endl;
	}
	return 0;
}

