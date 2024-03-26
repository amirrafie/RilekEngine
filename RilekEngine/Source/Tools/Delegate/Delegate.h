#pragma once

namespace Rilek
{
	// Delegates allow me to use the same function signature to store either member or non-member function
	template<typename... >
	class delegate;

	template<typename Ret, typename... TArgs>
	class delegate<Ret(TArgs...)>
	{
	public:
		delegate() : m_function_ptr(nullptr), m_instance(nullptr) {}

		template<auto Fn>
		void attach()
		{
			m_function_ptr = [](void*, TArgs... args)
			{
				return Ret(std::invoke(Fn, args...));
			};
		}

		template<auto Fn, typename T>
		void attach(T* instance)
		{
			m_instance = instance;
			m_function_ptr = [](void* instance, TArgs... args)
			{
				return Ret(std::invoke(Fn, static_cast<T*>(instance), args...));
			};
		}

		// calls the delegate
		Ret operator()(TArgs... args)
		{
			assert((m_function_ptr != nullptr) && "Delegate has no function attached!");
			return m_function_ptr(m_instance, args...);
		}

	private:
		typedef Ret(*function_pointer)(void*, TArgs...);
		function_pointer m_function_ptr;
		void* m_instance;
	};
}