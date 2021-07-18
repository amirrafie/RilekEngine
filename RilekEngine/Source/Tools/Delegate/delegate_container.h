#pragma once
#include "delegate.h"
#include "Tools/Containers/sparse_set.h"
namespace Rilek
{
	template<typename>
	class delegate_container;

	template<typename Ret, typename... TArgs>
	class delegate_container<Ret(TArgs...)> : public sparse_set<Rilek::delegate<Ret(TArgs...)>>
	{
		using delegate_id = size_t;
		delegate_id generate_delegate_ID()
		{
			static delegate_id delegate_ID = 0;
			return delegate_ID++;
		}

		// note, different delegate containers have different ids for the same function
		template<auto FN>
		delegate_id get_delegate_ID()
		{
			static delegate_id ID = generate_delegate_ID();
			return ID;
		}

	public:
		template<auto Fn>
		delegate_id add_delegate()
		{
			size_t ID = get_delegate_ID<Fn>();
			if (!contains(ID))
			{
				emplace(ID).attach<Fn>();
			}
			return ID;
		}

		template<auto Fn, typename T>
		delegate_id add_delegate(T* instance)
		{
			size_t ID = get_delegate_ID<Fn>();
			if (!contains(ID))
			{
				emplace(ID).attach<Fn>(instance);
			}
			return ID;
		}

		void call_delegates(TArgs... args)
		{
			for (auto& delegate : *this)
			{
				delegate(args);
			}
		}
	};
}