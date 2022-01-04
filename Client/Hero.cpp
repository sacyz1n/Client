#include "pch.h"
#include "Hero.h"
#include "Transform.h"
#include "PlayerController.h"
#include "Model.h"
#include "RenderingManager.h"

bool Hero::Initialize()
{
	// Components
	mTransform		  = AddComponent<Transform>();
	mTransform->Speed = 0.05f;

	mPlayerController = AddComponent<PlayerController>(); 
	mPlayerController->SetPawn(this);

	mModel			  = AddComponent<Model>();


	return true;
}

void Hero::Update()
{
	IGameObject::Update();

	RenderingManager::GetInstance()->AddRenderGroup(RenderGroup::TEST, this);
}

void Hero::LateUpdate()
{
	IGameObject::LateUpdate();
}

void Hero::Render()
{
#ifdef _DEBUG
	ConsoleRendering::Draw(_T("Position (%f, %f) "), mTransform->Position.X, mTransform->Position.Y);
#endif
	mModel->Render(mTransform->Position);
}

void Hero::Release()
{
	
}
