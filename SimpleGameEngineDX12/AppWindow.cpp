#include "AppWindow.h"

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	//Window::onCreate();
	GraphicsEngine::get()->init();

	swapChain = GraphicsEngine::get()->createSwapChain();
	RECT rc = this->getClientWindowRect();
	swapChain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);
}

void AppWindow::onUpdate()
{
	//Window::onUpdate();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	swapChain->release();
	GraphicsEngine::get()->release();
}
