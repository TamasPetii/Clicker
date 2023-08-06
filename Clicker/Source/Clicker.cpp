#include "Clicker.h"

int Clicker::mDelay = 25;
bool Clicker::mLeft = true;
bool Clicker::mRun = true;
std::thread Clicker::mThread;

void Clicker::Start()
{
	mThread = std::thread(Clicker::Callback);
}

void Clicker::Stop()
{
	mRun = false;

	if (mThread.joinable())
	{
		mThread.join();
	}
}

void Clicker::Callback()
{
	while (mRun)
	{
		while (GetAsyncKeyState(0x04))
		{
			if (mLeft)
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(mDelay));
			}
			else
			{
				mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(mDelay));
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(mDelay));
	}
}