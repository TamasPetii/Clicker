#pragma once
#include <Windows.h>
#include <thread>
#include <chrono>
#include <iostream>

class Clicker
{
public:
	static void Start();
	static void Stop();
	static int& GetDelayRef() { return mDelay; }
	static bool& GetLeftRef() { return mLeft; }
	static bool& GetChangedRef() { return mChanged; }
private:
	static void Callback();
	static int mDelay;
	static bool mLeft;
	static bool mRun;
	static bool mChanged;
	static std::thread mThread;
};

