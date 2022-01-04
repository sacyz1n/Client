#pragma once
#include "IGameObject.h"
#include "IComponent.h"

class PlayerController : public IComponent
{
public:
	PlayerController() = default;
	virtual ~PlayerController() = default;

	virtual void Update() final override;
	
	void SetPawn(IGameObject* _gameObject);

private:
	IGameObject* mPawn = nullptr;
};

