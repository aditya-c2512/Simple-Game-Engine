#include "AppWindow.h"
#include "InputSystem.h"

int main()
{
	try
	{
		GraphicsEngine::create();
		InputSystem::create();
	}
	catch (...)
	{
		return -1;
	}

	try
	{
		AppWindow app;
		while (app.isRun())//IF APP WINDOW IS CURRENTLY RUNNING
		{
			app.broadcast();
		}
	}
	catch (...)
	{
		GraphicsEngine::release();
		InputSystem::release();
		return -1;
	}

	GraphicsEngine::release();
	InputSystem::release();
	return 0;
}