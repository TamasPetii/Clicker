#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <fstream>
#include "Clicker.h"

struct Settings
{
	int mColorTheme = 2;
	bool mLeftActive = true;
	ImVec4 mButtonColor = ImVec4(1,0,0,1);
};

class App
{
public:
	static void Init();
	static void Run();
	static void Delete();
private:
	static void Render();
	static void PreRender();
	static void PostRender();
	static void RenderDockSpace();
	static void RenderDemo();
	static void RenderClicker();
	static void LoadSettings();
	static void SaveSettings();

	static void window_size_callback(GLFWwindow* window, int width, int height);
	static void Error(const std::string& pFunctionName);

	static GLFWwindow* mWindow;
	static Settings* mSettings;
};

