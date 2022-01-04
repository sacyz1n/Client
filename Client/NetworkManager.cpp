#include "pch.h"
#include "NetworkManager.h"

DEFINE_SINGLETON(NetworkManager)

NetworkManager::~NetworkManager()
{
	Release();
}

void NetworkManager::Update()
{

}

void NetworkManager::Release()
{
}
