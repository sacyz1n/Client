#pragma once

#include "IScene.h"

class SceneManager
{
	DECLARE_SINGLETON(SceneManager)

public:
	template <typename T>
	std::shared_ptr<IScene> CreateScene()
	{
		static_assert(!std::is_convertible_v<T, IScene>);

		return std::make_shared<T>();
	}

	void ChangeScene(std::shared_ptr<IScene> _scene)
	{
		if (!_scene->Initialize())
		{
			assert("Scene Initialization Failed...");
			return;
		}

		if (mCurScene != nullptr)
			mCurScene->Release();

		mCurScene = _scene;
	}

	void Update();
	void Release();

private:
	std::shared_ptr<IScene> mCurScene;
};

