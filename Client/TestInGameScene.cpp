#include "pch.h"
#include "TestInGameScene.h"
#include "Hero.h"

void TestInGameScene::Update()
{
	IScene::UpdateObject();
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
