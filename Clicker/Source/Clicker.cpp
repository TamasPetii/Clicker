#include "Clicker.h"

int Clicker::mDelay = 25;
bool Clicker::mLeft = true;
bool Clicker::mRun = true;
bool Clicker::mChanged = false;
std::thread Clicker::mThread;
std::thread Clicker::mGemMeltingThread;
std::atomic<bool> Clicker::mGemMeltingThreadRunning = false;

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
		while (GetAsyncKeyState(VK_MBUTTON))
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
		if (GetAsyncKeyState(VK_F1))
		{
			mLeft = true;
			mChanged = true;
		}
		if (GetAsyncKeyState(VK_F2))
		{
			mLeft = false;
			mChanged = true;
		}
		//Starting Gem Melting Thread 
		if (GetAsyncKeyState(VK_F4) && !mGemMeltingThreadRunning)
		{
			mGemMeltingThreadRunning = true;
			mGemMeltingThread = std::thread(Clicker::GemMeltingCallback);
			std::cout << "Started Gem Melting Thread!" << std::endl;
		}

		//Manually Stop Thread from working
		if (GetAsyncKeyState(VK_F3))
		{
			mGemMeltingThreadRunning = false;
			std::cout << "Stopped all thread manually!" << std::endl;
		}

		//Stopping Gem Melting Thread
		if (!mGemMeltingThreadRunning && mGemMeltingThread.joinable())
		{
			mGemMeltingThread.join();
			std::cout << "Gem Melting Thread Stopped" << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(mDelay));
	}
}

void Clicker::GemMeltingCallback()
{
	/*
	POINT currentPosition;
	if (GetCursorPos(&currentPosition))
	{
		std::cout << "Current Cursor Position: X=" << currentPosition.x << " Y=" << currentPosition.y << std::endl;
	}
	*/
	
	int start_x = 1370;
	int start_y = 640;

	for (int inventory = 0; inventory < 2; ++inventory)
	{
		//Inventory (1,2)
		SetCursorPos(1345 + inventory * 45, 570);
		ClickLeft();
		Sleep(100);

		for (int x = 0; x < 7; ++x)
		{
			if (inventory == 0 && x == 0)
				continue;

			for (int y = 0; y < 4; ++y)
			{
				if (!mGemMeltingThreadRunning)
					return;

				//Working bench position
				SetCursorPos(885, 520);
				ClickLeft();
				Sleep(100);

				//Gem position in the inventory
				SetCursorPos(start_x + x * 80, start_y + y * 75);
				ClickRight();
				Sleep(100);

				//Up icon
				SetCursorPos(545, 818);
				ClickLeft();
				Sleep(100);

				//- icon
				SetCursorPos(535, 796);
				ClickLeft();
				Sleep(100);

				//Combine icon
				SetCursorPos(700, 680);
				ClickLeft();
				Sleep(100);

				//Workbench X close icon
				SetCursorPos(810, 100);
				ClickLeft();

				Sleep(3000);
			}
		}
	}

	Sleep(1000);
	mGemMeltingThreadRunning = false;
}

void Clicker::ClickLeft()
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void Clicker::ClickRight()
{
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
}

void Clicker::Sleep(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}