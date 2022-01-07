#pragma once

#include "CoreCommon.h"

namespace GameCore
{
	class DLL_TYPE ReferenceCounter
	{
	public:
		int  Capture();
		void Release();

		virtual void OnAlloc() { }
		virtual void OnFree() { }

	private:
		std::atomic<int> mRefCnt = 0;
	};
}

