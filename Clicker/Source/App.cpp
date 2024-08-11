#include "App.h"

GLFWwindow* App::mWindow = nullptr;
Settings* App::mSettings = nullptr;

void App::window_size_callback(GLFWwindow* window, int width, int height)
{
    Render();
    glfwSwapBuffers(window);
}

void App::Error(const std::string& pFunctionName)
{
	std::cerr << "[" << pFunctionName << "] : Error occurred at line number " << __LINE__ << " in file: " << __FILE__ << std::endl;
	exit(1);
}

void App::Init()
{
	/*Initialize GLFW */
	if (!glfwInit()) Error("GLFW Initialization");

	/* Create Window */
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	mWindow = glfwCreateWindow(350, 250, "Clicker", NULL, NULL);
	if (!mWindow) Error("Window Creation");
    glfwSetWindowSizeCallback(mWindow, App::window_size_callback);

	/* Make the window's context current */
	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(1);

	/* ImGui */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
	ImGui_ImplOpenGL3_Init("#version 460");

    LoadSettings();
    Clicker::Start();
}

void App::Run()
{
	while (!glfwWindowShouldClose(mWindow))
	{
		glfwPollEvents();
		Render();
		glfwSwapBuffers(mWindow);
	}
}

void App::Delete()
{
    SaveSettings();
    Clicker::Stop();
    delete mSettings;
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void App::Render()
{
	PreRender();
    RenderDockSpace();
    RenderClicker();
    PostRender();
}


void App::PreRender()
{
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12);

    switch (mSettings->mColorTheme)
    {
    case 0: ImGui::StyleColorsDark(); break;
    case 1: ImGui::StyleColorsLight(); break;
    case 2: ImGui::StyleColorsClassic(); break;
    }

    if (Clicker::mChanged)
    {
        Clicker::mChanged = false;
        mSettings->mLeftActive = Clicker::mLeft;
    }
}

void App::PostRender()
{
    ImGui::PopStyleVar();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void App::RenderDockSpace()
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags =  ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace Demo", nullptr, window_flags);

    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::End();
}

void App::RenderDemo()
{
    ImGui::ShowDemoWindow();
}

void App::RenderClicker()
{
    bool leftActive = mSettings->mLeftActive;

    ImGui::Begin("Application", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

    //Left Button
    if (leftActive)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, mSettings->mButtonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, mSettings->mButtonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, mSettings->mButtonColor);
    }
    if (ImGui::Button("Left", ImVec2(ImGui::GetContentRegionAvail().x / 2, 120)))
    {
        mSettings->mLeftActive = true;
    }
    if (leftActive)
    {
        ImGui::PopStyleColor(3);
    }

    ImGui::SameLine();

    //Right Button
    if (!leftActive)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, mSettings->mButtonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, mSettings->mButtonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, mSettings->mButtonColor);
    }
    if (ImGui::Button("Right", ImVec2(ImGui::GetContentRegionAvail().x, 120)))
    {
        mSettings->mLeftActive = false;
    }
    if (!leftActive)
    {
        ImGui::PopStyleColor(3);
    }

    //Settings
    ImGui::SeparatorText("Settings");

    //Color Theme
    ImGui::Text("Color Theme:");
    ImGui::SameLine();
    ImGui::SetCursorPos(ImVec2(110, ImGui::GetCursorPos().y));
    if (ImGui::Combo("##ColorTheme", &mSettings->mColorTheme, "Dark\0Light\0Classic\0"))
    {
        switch (mSettings->mColorTheme)
        {
        case 0: ImGui::StyleColorsDark(); break;
        case 1: ImGui::StyleColorsLight(); break;
        case 2: ImGui::StyleColorsClassic(); break;
        }
    }

    //Button Color
    ImGui::Text("Button Color:");
    ImGui::SameLine();
    ImGui::SetCursorPos(ImVec2(110, ImGui::GetCursorPos().y));
    ImGui::ColorEdit3("##ButtonColor", &mSettings->mButtonColor.x);

    //Delay
    ImGui::Text("Delay (ms):");
    ImGui::SameLine();
    ImGui::SetCursorPos(ImVec2(110, ImGui::GetCursorPos().y));
    ImGui::SliderInt("##ClickDelay", &Clicker::mDelay, 1, 100);

    ImGui::End();

    Clicker::mLeft = mSettings->mLeftActive;
}

void App::LoadSettings()
{
    mSettings = new Settings();
    std::ifstream file("settings.txt");

    if (file.is_open())
    {
        file >> mSettings->mColorTheme;
        file >> mSettings->mButtonColor.x >> mSettings->mButtonColor.y >> mSettings->mButtonColor.z >> mSettings->mButtonColor.w;
        file >> Clicker::mDelay;
    }
}

void App::SaveSettings()
{
    std::ofstream file("settings.txt");

    if (file.is_open())
    {
        file << mSettings->mColorTheme << std::endl;
        file << mSettings->mButtonColor.x << " " << mSettings->mButtonColor.y << " " << mSettings->mButtonColor.z << " " << mSettings->mButtonColor.w << std::endl;
        file << Clicker::mDelay;
    }
}