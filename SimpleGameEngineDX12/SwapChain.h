#pragma once
#include <d3d11.h>
#include "Prerequisites_Header.h"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* rs);
	bool present(bool vsync);
	void resize(unsigned int width, unsigned int height);
	void setFullscreen(bool fullscreen, unsigned int width, unsigned int height);

	void reloadBuffers(unsigned int width, unsigned int height);

	~SwapChain();

private:
	IDXGISwapChain* m_swap_chain = nullptr;
	ID3D11RenderTargetView* m_rtv = nullptr;
	ID3D11DepthStencilView* m_dsv = nullptr;

	RenderSystem* render_system = nullptr;

	friend class DeviceContext;
};

