#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include <fstream>
#include "Vertex.h"
#include "Camera.h"

bool GraphicsEngine::Initialize(unsigned someX, unsigned someY,
	unsigned someWidth, unsigned someHeight,
	bool enableDeviceDebug)
{
	// Initialize our window:
	WNDCLASS windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = GraphicsEngine::WinProc;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = L"TGP";
	RegisterClass(&windowClass);

	myWindowHandle = CreateWindow(
		L"TGP",
		L"Till�mpad Grafikprogrammering",
		WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE,
		someX,
		someY,
		someWidth,
		someHeight,
		nullptr, nullptr, nullptr,
		this
		);

	// F1 -- This is where we should init our Framework
	if (!myFramework.Initialize(myWindowHandle, false))
	{
		return false;
	}
	
	if (!myForwardRenderer.Initialize())
	{
		return false;
	}
	
	if (!InitializeScene())
	{
		return false;
	}

	return true;
}

bool GraphicsEngine::InitializeScene()
{	
	myScene = std::make_shared<Scene>();
	auto camera = std::make_shared<Camera>();
	
	camera->SetPosition(0.0f, 0.0f, -5.0f);
	
	myScene->SetMainCamera(camera);
	
	return true;
}

LRESULT CALLBACK GraphicsEngine::WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// We want to be able to access the Graphics Engine instance from inside this function.
	static GraphicsEngine* graphicsEnginePtr = nullptr;

	if(uMsg == WM_DESTROY || uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}
	else if (uMsg == WM_CREATE)
	{
		const CREATESTRUCT* createdStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		graphicsEnginePtr = static_cast<GraphicsEngine*>(createdStruct->lpCreateParams);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void GraphicsEngine::BeginFrame()
{
	// F1 - This is where we clear our buffers and start the DX frame.
	myFramework.BeginFrame();
}

void GraphicsEngine::RenderFrame()
{
	// Will be fleshed out later!
	if (myScene)
	{
		const std::shared_ptr<Camera> camera = myScene->GetMainCamera();
		const std::vector<std::shared_ptr<Model>> modelsToRender = myScene->CullModels(camera);
		myForwardRenderer.Render(camera, modelsToRender);
	}
}

void GraphicsEngine::EndFrame()
{
	// F1 - This is where we finish our rendering and tell the framework
	// to present our result to the screen.
	myFramework.EndFrame();
}
