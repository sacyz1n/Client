#pragma once

#include "IScene.h"

class TestInGameScene : public IScene
{
public:
	virtual bool Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Release() override;
};

