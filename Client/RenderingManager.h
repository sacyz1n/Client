#pragma once

#include "IGameObject.h"

enum class RenderGroup
{
	BACKGROUND,
	TEST,
	UI,
	End
};

class RenderingManager
{
	DECLARE_SINGLETON(RenderingManager)

public:
	bool Initialize();
	void Render();
	void Release();

	void AddRenderGroup(RenderGroup _type, IGameObject* _gameObject);

private:
	using RENDERING_GROUP = std::list<IGameObject*>;
	using RENDERING_GROUPS = std::vector<RENDERING_GROUP>;
	RENDERING_GROUPS mRenderGroups;
};

