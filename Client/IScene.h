#pragma once

#include "IGameObject.h"

class IScene abstract 
{
public:
	explicit IScene() = default;
	virtual ~IScene() = default;

	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Release() = 0;

	void AddGameObject(GameObjectPtr _gameObject);
	void RemoveGameObject(GameObjectPtr _gameObject);

protected:
	void UpdateObject();
	void LateUpdateObject();

private:
	// numbering
	using NUMBER_OBJECTS = std::unordered_map<int, GameObjectPtr>;
	using NORMAL_OBJECTS = std::list<GameObjectPtr>;

	static NUMBER_OBJECTS mNumStaticObjects;
	NUMBER_OBJECTS 		  mNumDynamicObjects;

	// non numbering
	static NORMAL_OBJECTS	mNormalStaticObjects;
	NORMAL_OBJECTS			mNormalDynamicObjects;

	std::list<GameObjectPtr> mGameObjects;
};

