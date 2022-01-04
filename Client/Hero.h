#pragma once
#include "IGameObject.h"

class Hero : public IGameObject
{
public:
	Hero()
		: IGameObject(ObjectType::DYNAMIC_NORMAL)
	{ }

	virtual ~Hero()
	{
	}

	virtual bool Initialize() final override;
	virtual void Update() final override;
	virtual void LateUpdate() final override;
	virtual void Render() final override;
	virtual void Release() final override;

private:
	std::shared_ptr<class Transform>		mTransform;
	std::shared_ptr<class PlayerController> mPlayerController;
	std::shared_ptr<class Model>			mModel;
};


