#pragma once

#include <type_traits>
#include <memory>
#include <vector>

namespace GameCore
{
	template <typename T, int POOL_SIZE = 4096>
	class LightPool
	{
	public:
		using PoolObject = typename std::is_convertible<T, PoolObject>::value_type;
		using PoolObjectPtr = std::shared_ptr<PoolObject>;
		using PoolObjectArray = std::vector<PoolObjectPtr>;
		using PoolObjectStack = std::stack<PoolObjectPtr>;

		LightPool();
		~LightPool();

		PoolObjectPtr	Pop();
		void			Push(PoolObjectPtr _ptr);
		void			Resize(int _capacity);
		const size_t	Size() constexpr;
		const size_t	Capacity() constexpr;

	private:
		PoolObjectArray mPoolObjectArr;
		PoolObjectStack mPoolObjectStack;

		size_t mSize	 = 0;
		size_t mCapacity = 0;
	};
}