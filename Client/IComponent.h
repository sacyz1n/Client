#pragma once

class IComponent abstract
{
public:
	IComponent() = default;
	virtual ~IComponent() = default;

	virtual void Update() {}
	virtual void LateUpdate() {}
};

using ComPtr = std::shared_ptr<IComponent>;
