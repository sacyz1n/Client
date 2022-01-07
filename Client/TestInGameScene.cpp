#include "pch.h"
#include "TestInGameScene.h"
#include "Hero.h"
#include "NetworkManager.h"

void TestInGameScene::Update()
{
	IScene::UpdateObject();

	if (!NetworkManager::GetInstance()->IsConnect())
		NetworkManager::GetInstance()->TryConnect("127.0.0.1", 12021);
	else
		std::cout << "연결 성공" << std::endl;

}

void TestInGameScene::LateUpdate()
{
	IScene::LateUpdateObject();
}

bool TestInGameScene::Initialize()
{
	// Test 
	{
		auto hero = std::make_shared<Hero>();

		if (!hero->Initialize())
			return false;

		IScene::AddGameObject(hero);
	}

    return true;
}

void TestInGameScene::Release()
{
	
}
