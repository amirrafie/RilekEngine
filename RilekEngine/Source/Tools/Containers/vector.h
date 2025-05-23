#pragma once

namespace Rilek
{
	template <typename T>
	class vector
	{
	private:
		T* m_data;
		size_t m_size, m_capacity;

		static const size_t s_type_size = sizeof(T);

	public:
		using iterator = T*;
		using const_iterator = const T*;

		vector()
			: m_data(nullptr)
			, m_size(0)
			, m_capacity(0)
		{
		}

		vector(const vector& other)
			: m_size(other.m_size)
			, m_capacity(other.m_capacity)
		{
			m_data = new T[m_size];
			for (size_t i = 0; i < m_size; ++i)
				m_data[i] = other.m_data[i];
		}

		vector(vector&& other)
			: m_data(other.m_data)
			, m_size(other.m_size)
			, m_capacity(other.m_capacity)
		{
			other.m_data = nullptr;
			other.m_size = other.m_capacity = 0;
		}

		~vector()
		{
			clear();
			::operator delete(m_data);
		}

		vector& operator=(const vector &other)
		{
			if (m_capacity < other.m_capacity)
				reserve(other.m_capacity);
			clear();

			m_size = other.m_size;
			for (size_t i = 0; i < m_size; ++i)
				m_data[i] = other.m_data[i];
		}

		vector& operator=(vector&& other)
		{
			m_data = other.m_data;
			m_size = other.m_size;
			m_capacity = other.m_capacity;
			other.m_data = nullptr;
			other.m_size = other.m_capacity = 0;
		}

		// Capacity
		void reserve(size_t new_capacity)
		{
			if (m_capacity >= new_capacity)
				return;
			T *new_data = static_cast<T*>(::operator new(s_type_size * new_capacity));
			for (size_t i = 0; i < m_size; ++i)
			{
				new (new_data + i) T(std::move(m_data[i]));
				m_data[i].~T();
			}
			::operator delete(m_data);
			m_data = new_data;
			m_capacity = new_capacity;
		}

		size_t capacity() const
		{
			return m_capacity;
		}

		size_t size() const
		{
			return m_size;
		}

		// Modifiers
		void clear()
		{
			for (size_t i = 0; i < m_size; ++i)
				m_data[i].~T();
			m_size = 0;
		}

		void resize(size_t count)
		{
			if (count < m_size)
			{
				for (size_t i = count; i < m_size; ++i)
					m_data[i].~T();
			}
			else
			{
				if (count > m_capacity)
					reserve(count);
				for (size_t i = m_size; i < count; ++i)
					new (m_data + i) T();
			}
			m_size = count;
		}

		void resize(size_t count, const T& value)
		{
			if (count < m_size)
			{
				for (size_t i = count; i < m_size; ++i)
					m_data[i].~T();
			}
			else
			{
				if (count > m_capacity)
					reserve(count);
				for (size_t i = m_size; i < count; ++i)
					new (m_data + i) T(value);
			}
			m_size = count;
		}

		void push_back(const T& value)
		{
			if (m_size == m_capacity)
				grow();
			new (m_data + m_size) T(value);
			++m_size;
		}

		T& push_back(T&& value)
		{
			if (m_size == m_capacity)
				grow();
			new (m_data + m_size) T(value);
			return *(m_data + m_size++);
		}

		template <typename... TArgs>
		T& emplace_back(TArgs&&... args)
		{
			if (m_size == m_capacity)
				grow();
			new (m_data + m_size) T(std::forward<TArgs>(args)...);
			return *(m_data + m_size++);
		}

		void pop_back()
		{
			if (m_size)
			{
				m_data[m_size--].~T();
			}
		}

		// iteration
		iterator begin()
		{
			return m_data;
		}

		iterator end()
		{
			return m_data + m_size;
		}

		const_iterator cbegin() const
		{
			return m_data;
		}

		const_iterator cend() const
		{
			return m_data + m_size;
		}

		// Access
		T& operator[](size_t index)
		{
			return *(m_data + index);
		}

		const T& operator[](size_t index) const
		{
			return *(m_data + index);
		}

		T& back()
		{
			return *(m_data + m_size);
		}

		const T& back() const
		{
			return *(m_data + m_size);
		}

	private:
		// Using this function to maintain our growing policy during push/emplace back
		void grow()
		{
			reserve(m_capacity ? (m_capacity * 2) : 2);
		}
	};
}