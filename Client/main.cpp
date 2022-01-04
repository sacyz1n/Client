// Client.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "SystemManager.h"
#include "SceneManager.h"
#include "TestInGameScene.h"
#include "RenderingManager.h"
#include "TimerManager.h"
#include "NetworkManager.h"
#include "InputManager.h"


int _tmain()
{
	auto systemManager = SystemManager::GetInstance();
	auto sceneManager = SceneManager::GetInstance();
	auto renderingManager = RenderingManager::GetInstance();
	auto timerManager = TimerManager::GetInstance();
	auto networkManager = NetworkManager::GetInstance();
	auto inputManager = InputManager::GetInstance();

	
	// Initialize RenderingManager
	if (!renderingManager->Initialize())
		assert("Error - renderingManager->Initialize()");

	// Initialize ConsoleRendering
	ConsoleRendering::Initialize({150, 60});

	// Initialize Scene
	{
		auto scene = sceneManager->CreateScene<TestInGameScene>();
		sceneManager->ChangeScene(scene);
	}

	// Initialize Frame Rate
	systemManager->SetFrameRate(5.f);

	// Initialize Timer Manager
	if (!timerManager->Initialize())
		assert("Error - timerManager->Initialize()");

	// Game Loop
	while (!systemManager->IsTrue(SYSTEM_OPTION::SO_QUIT_APPLICATION))
	{
		// Update Timer
		timerManager->Update();

		// Limit Frame
		if (systemManager->GetFrameTime() <= timerManager->GetAccTime())
		{
#ifdef _DEBUG
			// Count FPS
			systemManager->FPSCounter();
			systemManager->FPSTimeAdd(DELTA_TIME);
#endif
			// Update Network
			networkManager->Update();

			// Update Input
			inputManager->Update();

			// Update Scene
			sceneManager->Update();

			// Render Object
			renderingManager->Render();

			// Reset AccTime
			timerManager->ResetAccTime();
		}
	}

	// Release
	InputManager::DestroyInstance();
	TimerManager::DestroyInstance();
	RenderingManager::DestroyInstance();
	SceneManager::DestroyInstance();
	NetworkManager::DestroyInstance();
	ConsoleRendering::Release();
	return 0;
}
