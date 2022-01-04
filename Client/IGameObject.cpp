#include "pch.h"
#include "IGameObject.h"

IGameObject::~IGameObject()
{
	Release();

	mComponents.clear();
}

void IGameObject::Update()
{
	for (auto& pair : mComponents)
		pair.second->Update();
}

void IGameObject::LateUpdate()
{
	for (auto& pair : mComponents)
		pair.second->LateUpdate();
}

void IGameObject::SetPos(int _x, int _y)
{
}
