#pragma once
#include <Windows.h>



class Window
{
public:
	Window();
	bool broadcast();
	bool isRun();

	RECT getClientWindowRect();

	//EVENTS
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

	~Window();

protected:
	HWND m_hwnd;
	bool m_is_run;
	bool m_is_init = false;
};

