#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "usp10.lib")


#include <wrl/client.h>
#include <string>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "GameTimer.h"
#include "Renderer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include <memory>
#include "Input.h"

#include "Common.h"

class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);              // 在构造函数的初始化列表应当设置好初始参数
	virtual ~D3DApp();

	HINSTANCE AppInst()const;                 // 获取应用实例的句柄
	HWND      MainWnd()const;                 // 获取主窗口句柄
	float     AspectRatio()const;             // 获取屏幕宽高比

	int Run();                                // 运行程序，进行游戏主循环

	// 框架方法。客户派生类需要重载这些方法以实现特定的应用需求
	virtual bool Init();                      // 该父类方法需要初始化窗口和Direct3D部分
	virtual void OnResize();                  // 该父类方法需要在窗口大小变动的时候调用
	virtual void UpdateScene(float dt) = 0;   // 子类需要实现该方法，完成每一帧的更新
	virtual void DrawScene() = 0;             // 子类需要实现该方法，完成每一帧的绘制
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// 窗口的消息回调函数
protected:
	bool InitMainWindow();      // 窗口初始化
	bool InitDirect3D();        // Direct3D初始化

	void CalculateFrameStats(); // 计算每秒帧数并在窗口显示
protected:

	HINSTANCE m_hAppInst;        // 应用实例句柄
	HWND      m_hMainWnd;        // 主窗口句柄
	bool      m_AppPaused;       // 应用是否暂停
	bool      m_Minimized;       // 应用是否最小化
	bool      m_Maximized;       // 应用是否最大化
	bool      m_Resizing;        // 窗口大小是否变化
	bool	  m_Enable4xMsaa;	 // 是否开启4倍多重采样
	UINT      m_4xMsaaQuality;   // MSAA支持的质量等级

	GameTimer m_Timer;           // 计时器

	// Direct3D 11
	ComPtr<ID3D11Device> _pd3dDevice;                    // D3D11设备
	ComPtr<ID3D11DeviceContext> _pd3dDeviceContext;   // D3D11设备上下文
	ComPtr<IDXGISwapChain> _pSwapChain;                  // D3D11交换链
	// Direct3D 11.1
	ComPtr<ID3D11Device1> _pd3dDevice1;                  // D3D11.1设备
	ComPtr<ID3D11DeviceContext1> _pd3dImmediateContext1; // D3D11.1设备上下文
	ComPtr<IDXGISwapChain1> _pSwapChain1;                // D3D11.1交换链
	// 常用资源
	ComPtr<ID3D11Texture2D> _pDepthStencilBuffer;        // 深度模板缓冲区
	ComPtr<ID3D11RenderTargetView> _pRenderTargetView;   // 渲染目标视图
	ComPtr<ID3D11DepthStencilView> _pDepthStencilView;   // 深度模板视图
	D3D11_VIEWPORT _ScreenViewport;                      // 视口

	std::unique_ptr<Renderer> _pRenderer;

	// 派生类应该在构造函数设置好这些自定义的初始参数
	std::wstring m_MainWndCaption;                       // 主窗口标题
	int m_ClientWidth;                                   // 视口宽度
	int m_ClientHeight;                                  // 视口高度

	// 键鼠输入
	std::unique_ptr<Input> _pInput;
};