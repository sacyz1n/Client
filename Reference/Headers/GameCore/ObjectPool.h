#pragma once


#include "FastSpinlock.h"

template <class TOBJECT, int ALLOC_COUNT = 100>
class ObjectPool : public ClassTypeLock<TOBJECT>
{
public:
	// 	ObjectPool()
	// 	{
	// 		static_assert(std::is_polymorphic<TOBJECT>::value == false, "NO VIRTUAL TOBJECT!");
	// 	}

	static void* operator new(size_t objSize)
	{
		using uint8_t = unsigned char;

		typename ClassTypeLock<TOBJECT>::LockGuard criticalSection;

		if (!mFreeList)
		{
			mFreeList = new unsigned char[sizeof(TOBJECT) * ALLOC_COUNT];

			auto* pNext = mFreeList;
			auto ppCurr = reinterpret_cast<unsigned char**>(mFreeList);

			for (int i = 0; i < ALLOC_COUNT - 1; ++i)
			{
				/// OBJECT�� ũ�Ⱑ �ݵ�� ������ ũ�⺸�� Ŀ�� �Ѵ�
				pNext += sizeof(TOBJECT);
				*ppCurr = pNext;
				ppCurr = reinterpret_cast<unsigned char**>(pNext);
			}

			*ppCurr = 0; ///< �������� 0���� ǥ��
			mTotalAllocCount += ALLOC_COUNT;
		}

		auto* pAvailable = mFreeList;
		mFreeList = *reinterpret_cast<unsigned char**>(pAvailable);
		++mCurrentUseCount;

		return pAvailable;
	}

	static void	operator delete(void* obj)
	{
		typename ClassTypeLock<TOBJECT>::LockGuard criticalSection;

		if (mCurrentUseCount <= 0)
		{
			ASSERT("mCurrentUseCount <= 0");
		}
		--mCurrentUseCount;

		*reinterpret_cast<unsigned char**>(obj) = mFreeList;
		mFreeList = static_cast<unsigned char*>(obj);
	}


private:
	static unsigned char* mFreeList;
	static int		mTotalAllocCount; ///< for tracing
	static int		mCurrentUseCount; ///< for tracing

};


template <class TOBJECT, int ALLOC_COUNT>
unsigned char* ObjectPool<TOBJECT, ALLOC_COUNT>::mFreeList = nullptr;

template <class TOBJECT, int ALLOC_COUNT>
int ObjectPool<TOBJECT, ALLOC_COUNT>::mTotalAllocCount = 0;

template <class TOBJECT, int ALLOC_COUNT>
int ObjectPool<TOBJECT, ALLOC_COUNT>::mCurrentUseCount = 0;


