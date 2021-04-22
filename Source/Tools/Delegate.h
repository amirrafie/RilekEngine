#pragma once
#include <functional>
#include <cassert>
namespace Tools
{
	template<typename... >
	class Delegate;

	template<typename Ret, typename... TArgs>
	class Delegate<Ret(TArgs...)>
	{
	public:
		Delegate() : m_funcPtr(nullptr), m_instance(nullptr) {}

		template<auto Fn>
		void Attach()
		{
			m_funcPtr = [](void*, TArgs... args)
			{
				return Ret(std::invoke(Fn, args...));
			};
		}

		template<auto Fn, typename T>
		void Attach(T* instance)
		{
			m_instance = instance;
			m_funcPtr = [](void* instance, TArgs... args)
			{
				return Ret(std::invoke(Fn, static_cast<T*>(instance), args...));
			};
		}

		Ret operator()(TArgs... args)
		{
			assert((m_funcPtr != nullptr) && "Delegate has no function attached!");
			return m_funcPtr(m_instance, args...);
		}

	private:
		typedef Ret(*FuncPtr)(void*, TArgs...);
		FuncPtr m_funcPtr;
		void* m_instance;
	};
}