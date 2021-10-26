#pragma once
#include "Window.h"
static class Application
{
public:

	static void init(WindowSettings ws);
	static void ChangeWindowSettings(WindowSettings ws);
	static bool ShouldClose();

	
	static void PreRender();
	static void RenderImGui();
	static void FlushFrameBuffer();
	static void Terminate();
	static void DrawDebug();
private:


	static bool OpenGlActive;
	
	static void InitOpenGL(int VersionMinor, int VersionMajor);

	static void InitImGui();

	
	
};

