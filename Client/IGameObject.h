#pragma once

#include "pch.h"
#include "IComponent.h"


using TypeInfoRef = std::reference_wrapper<const std::type_info>;

class IGameObject
{
public:
	// Constructor & Destructor
	IGameObject(ObjectType _type, const int _objectIndex)
		: mObjectType(_type)
		, mObjectIndex(_objectIndex)
	{ }
	IGameObject(ObjectType _type)
		: mObjectType(_type)
	{ }
	virtual ~IGameObject();

	// Virtual Function
	virtual bool Initialize() = 0;
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render() = 0;
	virtual void Release() {}

	// General Function
	void SetPos(int _x, int _y);

	struct Hasher {
		std::size_t operator()(TypeInfoRef code) const
		{
			return code.get().hash_code();
		}
	};
	struct EqualTo {
		bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const
		{
			return lhs.get() == rhs.get();
		}
	};
	using COMPONENTS = std::unordered_map<TypeInfoRef, ComPtr, Hasher, EqualTo>;

	// Template Function
	template <typename T>
	std::shared_ptr<T> GetComponent()
	{
		static_assert(!std::is_convertible_v<T, IComponent>);

		auto findIter = mComponents.find(typeid(T));

		if (findIter == mComponents.end())
			return nullptr;

		return std::static_pointer_cast<T>(findIter->second);
	}

	template <typename T>
	std::shared_ptr<T> AddComponent()
	{
		static_assert(!std::is_convertible_v<T, IComponent>);

		TypeInfoRef typeId = typeid(T);

		auto findIter = mComponents.find(typeId);

		if (findIter != mComponents.end())
			return nullptr;

		auto component = std::make_shared<T>();
		mComponents.insert(COMPONENTS::value_type(typeId, component));
		return component;
	}

	const int& Index() const { return mObjectIndex; }
	const bool IsDestroy() const { return mIsDestroy; }
	const ObjectType GetObjectType() const { return mObjectType; }

protected:
	

private:
	COMPONENTS mComponents;
	ObjectType mObjectType;
	const int& mObjectIndex = -1;
	bool mIsDestroy = false;
};

using GameObjectPtr = std::shared_ptr<IGameObject>;