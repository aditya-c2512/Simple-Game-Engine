#pragma once
#include <Windows.h>

class Window
{
public :
	Window();
	// Initialise the window
	bool init();
	bool broadcast();
	// Release or destroy the window
	bool release();
	bool isRun();
	~Window();

	//EVENTS
	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy();

protected :
	HWND m_hwnd;
	bool m_is_run;
};

