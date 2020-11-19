#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")


#include <wrl/client.h>
#include <string>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "GameTimer.h"
#include "Renderer.h"
#include <memory>

#include "Common.h"

class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);              // �ڹ��캯���ĳ�ʼ���б�Ӧ�����úó�ʼ����
	virtual ~D3DApp();

	HINSTANCE AppInst()const;                 // ��ȡӦ��ʵ���ľ��
	HWND      MainWnd()const;                 // ��ȡ�����ھ��
	float     AspectRatio()const;             // ��ȡ��Ļ��߱�

	int Run();                                // ���г��򣬽�����Ϸ��ѭ��

	// ��ܷ������ͻ���������Ҫ������Щ������ʵ���ض���Ӧ������
	virtual bool Init();                      // �ø��෽����Ҫ��ʼ�����ں�Direct3D����
	virtual void OnResize();                  // �ø��෽����Ҫ�ڴ��ڴ�С�䶯��ʱ�����
	virtual void UpdateScene(float dt) = 0;   // ������Ҫʵ�ָ÷��������ÿһ֡�ĸ���
	virtual void DrawScene() = 0;             // ������Ҫʵ�ָ÷��������ÿһ֡�Ļ���
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// ���ڵ���Ϣ�ص�����
protected:
	bool InitMainWindow();      // ���ڳ�ʼ��
	bool InitDirect3D();        // Direct3D��ʼ��

	void CalculateFrameStats(); // ����ÿ��֡�����ڴ�����ʾ
protected:

	HINSTANCE m_hAppInst;        // Ӧ��ʵ�����
	HWND      m_hMainWnd;        // �����ھ��
	bool      m_AppPaused;       // Ӧ���Ƿ���ͣ
	bool      m_Minimized;       // Ӧ���Ƿ���С��
	bool      m_Maximized;       // Ӧ���Ƿ����
	bool      m_Resizing;        // ���ڴ�С�Ƿ�仯
	bool	  m_Enable4xMsaa;	 // �Ƿ���4�����ز���
	UINT      m_4xMsaaQuality;   // MSAA֧�ֵ������ȼ�

	GameTimer m_Timer;           // ��ʱ��

	// Direct3D 11
	ComPtr<ID3D11Device> _pd3dDevice;                    // D3D11�豸
	ComPtr<ID3D11DeviceContext> _pd3dDeviceContext;   // D3D11�豸������
	ComPtr<IDXGISwapChain> _pSwapChain;                  // D3D11������
	// Direct3D 11.1
	ComPtr<ID3D11Device1> _pd3dDevice1;                  // D3D11.1�豸
	ComPtr<ID3D11DeviceContext1> _pd3dImmediateContext1; // D3D11.1�豸������
	ComPtr<IDXGISwapChain1> _pSwapChain1;                // D3D11.1������
	// ������Դ
	ComPtr<ID3D11Texture2D> _pDepthStencilBuffer;        // ���ģ�建����
	ComPtr<ID3D11RenderTargetView> _pRenderTargetView;   // ��ȾĿ����ͼ
	ComPtr<ID3D11DepthStencilView> _pDepthStencilView;   // ���ģ����ͼ
	D3D11_VIEWPORT _ScreenViewport;                      // �ӿ�

	std::unique_ptr<Renderer> _pRenderer;

	// ������Ӧ���ڹ��캯�����ú���Щ�Զ���ĳ�ʼ����
	std::wstring m_MainWndCaption;                       // �����ڱ���
	int m_ClientWidth;                                   // �ӿڿ��
	int m_ClientHeight;                                  // �ӿڸ߶�
};
