#include "pch.h"
#include "SceneManager.h"

DEFINE_SINGLETON(SceneManager)

SceneManager::~SceneManager()
{
	Release();
}

void SceneManager::Update()
{
	if (mCurScene == nullptr)
		return;

	mCurScene->Update();
	mCurScene->LateUpdate();
}

void SceneManager::Release()
{
}
