#include "pch.h"
#include "RenderingManager.h"

DEFINE_SINGLETON(RenderingManager)

RenderingManager::~RenderingManager()
{
	Release();
}

bool RenderingManager::Initialize()
{
	mRenderGroups.resize(static_cast<size_t>(RenderGroup::End));

	return true;
}

void RenderingManager::Render()
{
	ConsoleRendering::ClearBuffer();
	
	// Test
	ConsoleRendering::Draw(_T("%S \n"), SystemManager::GetInstance()->GetFPS());
	ConsoleRendering::Draw(_T("DeltaTime : %f\n"), DELTA_TIME);
	ConsoleRendering::Draw(_T("SystemTime : %f\n"), SYSTEM_TIME);

	for (auto& renderGroup : mRenderGroups)
	{
		for (auto& gameObject : renderGroup)
			gameObject->Render();

		renderGroup.clear();
	}

	ConsoleRendering::SwapBuffer();
}

void RenderingManager::Release()
{
	for (auto& renderGroup : mRenderGroups)
		renderGroup.clear();

	mRenderGroups.clear();
	mRenderGroups.shrink_to_fit();
}

void RenderingManager::AddRenderGroup(RenderGroup _type, IGameObject* _gameObject)
{
	if (_type >= RenderGroup::End || static_cast<int>(_type) < 0)
		return;

	mRenderGroups[static_cast<int>(_type)].push_back(_gameObject);
}
