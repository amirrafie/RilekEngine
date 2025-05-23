#pragma once
#include <iterator>

namespace Rilek
{
	template <typename T>
	class reverse
	{

	private:
		T& m_container;

	public:
		using iterator = std::reverse_iterator<typename T::iterator>;
		using const_iterator = std::reverse_iterator<typename T::const_iterator>;


		reverse(T& t_container)
			: m_container(t_container)
		{
		}

		iterator begin()
		{
			return iterator(m_container.end());
		}

		iterator end()
		{
			return iterator(m_container.begin());
		}

		const_iterator cbegin() const
		{
			return const_iterator(m_container.cend());
		}

		const_iterator cend() const
		{
			return const_iterator(m_container.cbegin());
		}
	};
}