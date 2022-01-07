#pragma once

namespace GameCore
{
	class IComponent abstract
	{
	public:
		virtual void OnCreate() { }
		virtual void OnDestroy() { }
	};
}

