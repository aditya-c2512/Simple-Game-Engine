#include "Window.h"

Window* window;

Window::Window()
{
}

LRESULT CALLBACK WndPrc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		window->onCreate();
		break;
	}
	case WM_DESTROY:
	{
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}
	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

bool Window::init()
{
	WNDCLASSEX wcex;
	wcex.cbClsExtra = NULL;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbWndExtra = NULL;
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = NULL;
	wcex.lpszClassName = (LPCWSTR)"MyWindow";
	wcex.lpszMenuName = (LPCWSTR)"";
	wcex.style = NULL;
	wcex.lpfnWndProc = &WndPrc;

	if(!::RegisterClassEx(&wcex)) return false;

	if (!window) window = this;
	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, (LPCWSTR)"MyWindow", (LPCWSTR)"DirectX Engine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, NULL);
	if (!m_hwnd) return false;

	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	m_is_run = true;

	return true;
}

bool Window::broadcast()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	window->onUpdate();
	Sleep(0);
	return false;
}

bool Window::release()
{
	if (::DestroyWindow(m_hwnd)) return false;
	return true;
}

bool Window::isRun()
{
	return m_is_run;
}

Window::~Window()
{
}

void Window::onDestroy()
{
	m_is_run = false;
}
