#pragma once
#include <Windows.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <atomic>

class Clicker
{
public:
	static void Start();
	static void Stop();

	static int mDelay;
	static bool mLeft;
	static bool mRun;
	static bool mChanged;
private:
	static void Sleep(int ms);
	static void ClickLeft();
	static void ClickRight();


	static void Callback();
	static std::thread mThread;

	static std::atomic<bool> mGemMeltingThreadRunning;
	static void GemMeltingCallback();
	static std::thread mGemMeltingThread;
};

