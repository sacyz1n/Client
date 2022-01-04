#include "pch.h"
#include "PlayerController.h"
#include "InputManager.h"
#include "Transform.h"

void PlayerController::Update()
{
	if (mPawn == nullptr)
		return;

	auto transform = mPawn->GetComponent<Transform>();
	if (transform == nullptr)
		return;

	auto inputManager = InputManager::GetInstance();
	if (inputManager->IsKeyPressing(InputKey::KeyUp))
	{
		transform->Move(MoveDirection::Up);
	}

	if (inputManager->IsKeyPressing(InputKey::KeyDown))
	{
		transform->Move(MoveDirection::Down);
	}

	if (inputManager->IsKeyPressing(InputKey::KeyRight))
	{
		transform->Move(MoveDirection::Right);
	}

	if (inputManager->IsKeyPressing(InputKey::KeyLeft))
	{
		transform->Move(MoveDirection::Left);
	}
}

void PlayerController::SetPawn(IGameObject* _gameObject)
{
	mPawn = _gameObject;
}
