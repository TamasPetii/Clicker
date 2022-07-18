#include <iostream>
#include <Windows.h>


int main() {

	bool left = true;

	while (true) {
		
		while (GetAsyncKeyState(0x04)) {
			if (left) {
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				Sleep(25);
			}
			else {
				mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
				Sleep(25);
			}
		}
		
		if (GetAsyncKeyState(0x70)) {
			left = true;
		}
		if (GetAsyncKeyState(0x71)) {
			left = false;
		}
		Sleep(100);
	}
}