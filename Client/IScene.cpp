#include "pch.h"
#include "IScene.h"

std::unordered_map<int, GameObjectPtr> IScene::mNumStaticObjects;
std::list<GameObjectPtr> IScene::mNormalStaticObjects;

void IScene::AddGameObject(GameObjectPtr _gameObject)
{
	if (_gameObject == nullptr)
		return;

	switch (_gameObject->GetObjectType())
	{
	case ObjectType::DYNAMIC_NUMBERING:
		{
			const auto& index = _gameObject->Index();
			auto iter = mNumDynamicObjects.find(index);

			if (mNumDynamicObjects.end() != iter)
				return;

			mNumDynamicObjects.emplace(index, _gameObject);
		}
		break;
		
	case ObjectType::STATIC_NUMBERING:
		{
			const auto& index = _gameObject->Index();
			auto iter = mNumStaticObjects.find(index);
			
			if (mNumStaticObjects.end() != iter)
				return;

			mNumStaticObjects.emplace(index, _gameObject);
		}
		break;
	case ObjectType::DYNAMIC_NORMAL:
		mNormalDynamicObjects.push_back(_gameObject);
		break;
	case ObjectType::STATIC_NORMAL:
		mNormalStaticObjects.push_back(_gameObject);
		break;
	}

	mGameObjects.push_back(_gameObject);
}

void IScene::RemoveGameObject(GameObjectPtr _gameObject)
{
	switch (_gameObject->GetObjectType())
	{
	case ObjectType::DYNAMIC_NUMBERING:
		{
			auto iter = mNumDynamicObjects.find(_gameObject->Index());

			if (iter != mNumDynamicObjects.end())
				mNumDynamicObjects.erase(iter);
		}
		break;
	case ObjectType::STATIC_NUMBERING:
		{
			auto iter = mNumStaticObjects.find(_gameObject->Index());

			if (iter != mNumStaticObjects.end())
				mNumStaticObjects.erase(iter);
		}
		break;
	case ObjectType::DYNAMIC_NORMAL:
		{
			auto iter = std::find(mNormalDynamicObjects.begin(), mNormalDynamicObjects.end(), _gameObject);

			if (iter != mNormalDynamicObjects.end())
				mNormalDynamicObjects.erase(iter);
		}
		break;
	case ObjectType::STATIC_NORMAL:
		{
			auto iter = std::find(mNormalStaticObjects.begin(), mNormalStaticObjects.end(), _gameObject);

			if (iter != mNormalStaticObjects.end())
				mNormalStaticObjects.erase(iter);
		}
		break;
	}
}

void IScene::UpdateObject()
{
	auto iterEnd = mGameObjects.end();
	for (auto iter = mGameObjects.begin(); iter != iterEnd; )
	{
		auto gameObject = (*iter);

		if (gameObject == nullptr)
		{
			iter = mGameObjects.erase(iter);
			continue;
		}

		// Update
		gameObject->Update();

		if (gameObject->IsDestroy())
		{
			RemoveGameObject(gameObject);
			iter = mGameObjects.erase(iter);
			continue;
		}

		++iter;
	}

}

void IScene::LateUpdateObject()
{
	for (auto& gameObject : mGameObjects)
		gameObject->LateUpdate();
}
