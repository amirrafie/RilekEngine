#pragma once

#include <vector>	// vector
namespace Rilek
{
	template<typename T>
	class sparse_set
	{
	public:
		//////////////////////
		// ITERATOR
		//////////////////////

		// use vetor iterator for ranges
		auto begin()
		{
			return m_dense.begin();
		}

		auto end()
		{
			return m_dense.end();
		}

		auto begin() const
		{
			return m_dense.cbegin();
		}

		auto end() const
		{
			return m_dense.cend();
		}

		auto cbegin() const
		{
			return m_dense.cbegin();
		}

		auto cend() const
		{
			return m_dense.cend();
		}


		//////////////////////
		// CAPACITY
		//////////////////////
		bool empty() const
		{
			return !(m_size);
		}

		size_t size() const
		{
			return m_size;
		}

		void reserve(size_t t_new_capacity)
		{
			// grow the capacity if needed
			if (t_new_capacity > m_capcity)
			{
				m_capcity = t_new_capacity;
				m_sparse.resize(m_capcity, empty_index);
				m_dense.reserve(m_capcity);
				m_sparse_indices.reserve(m_capcity);
			}
		}

		size_t capacity() const
		{
			return m_capcity;
		}

		//////////////////////
		// MODIFIERS
		//////////////////////

		void clear()
		{
			m_sparse.clear();
			m_dense.clear();
			m_sparse_indices.clear();
			m_size = 0;
		}

		// copy element in place in indexed position
		T& insert(size_t t_index, const T& t_value)
		{
			// reserve capacity if needed for the specific index
			if (t_index >= m_capcity)
			{
				reserve(t_index + 1);
			}

			// if element is currently empty, insert a new element at the index
			if (m_sparse[t_index] == empty_index)
			{
				m_sparse[t_index] = m_dense.size();
				m_dense.push_back(t_value);
				m_sparse_indices.emplace_back(t_index);
				++m_size;
			}
			// if element already in the container, just construct and throw away (following stl behaviour)
			else
			{
				
				T temp(t_value);
				(void)temp;
			}
			return m_dense[m_sparse[t_index]];
		}

		// copy element in place in indexed position
		T& insert_or_assign(size_t t_index, const T& t_value)
		{
			// reserve capacity if needed for the specific index
			if (t_index >= m_capcity)
			{
				reserve(t_index + 1);
			}

			// if element is currently empty. insert the new element in index
			if (m_sparse[t_index] == empty_index)
			{
				m_sparse[t_index] = m_dense.size();
				m_dense.push_back(t_value);
				m_sparse_indices.emplace_back(t_index);
				++m_size;
			}
			// if element already in the container, assign it
			else
			{
				m_dense[m_sparse[t_index]] = t_value;
			}
			return m_dense[m_sparse[t_index]];
		}

		// create element in place in indexed position
		template<typename... TArgs>
		T& emplace(size_t t_index, TArgs&&... t_args)
		{
			// reserve capacity if needed for the specific index
			if (t_index >= m_capcity)
			{
				reserve(t_index + 1);
			}

			// if element is currently empty. construct the new element in index
			if (m_sparse[t_index] == empty_index)
			{
				m_sparse[t_index] = m_dense.size();
				m_dense.emplace_back(std::forward<TArgs>(t_args)...);
				m_sparse_indices.emplace_back(t_index);
				++m_size;
			}
			else
			{
				// if element already in the container, just construct and throw away (following stl behaviour)
				T temp(std::forward<TArgs>(t_args)...);
				(void)temp;
			}
			return m_dense[m_sparse[t_index]];
		}


		void erase(size_t t_index)
		{
			if (!m_size || t_index >= m_capcity || m_sparse[t_index] == empty_index)
				return;

			// index to dense array
			size_t dense_index = m_sparse[t_index];

			// sparse index of last element in dense array
			size_t last_sparse_index = m_sparse_indices[m_size - 1];

			// swap elements
			std::swap(m_dense[dense_index], m_dense[m_size - 1]);
			std::swap(m_sparse_indices[dense_index], m_sparse_indices[m_size - 1]);

			// point the sparse index of the previously last element in dense array to the new position in dense array
			m_sparse[last_sparse_index] = dense_index;

			// delete last element in dense array
			m_dense.pop_back();
			m_sparse_indices.pop_back();

			// mark element as empty
			m_sparse[t_index] = empty_index;

			--m_size;
		}

		//////////////////////
		// LOOKUP
		//////////////////////

		// access
		T& at(size_t t_index)
		{
			if (t_index >= m_capcity || m_sparse[t_index] == empty_index)
			{
				throw std::out_of_range("sparse_set out of range!");
			}
			return m_dense[m_sparse[t_index]];
		}

		// access (const)
		const T& at(size_t t_index) const
		{
			if (t_index >= m_capcity || m_sparse[t_index] == empty_index)
			{
				throw std::out_of_range("sparse_set out of range!");
			}
			return m_dense[m_sparse[t_index]];
		}

		// access or inserts
		T& operator[](size_t t_index)
		{
			if (!contains(t_index))
			{
				emplace(t_index);
			}
			return m_dense[m_sparse[t_index]];
		}

		bool contains(size_t t_index) const
		{
			return (t_index < m_capcity&& m_sparse[t_index] != empty_index);
		}

	private:
		static const size_t empty_index = std::numeric_limits<size_t>::max();

		std::vector<size_t> m_sparse;
		std::vector<T> m_dense;
		std::vector<size_t> m_sparse_indices;	// 1 to 1 relation to m_dense. to use to keep track of the index used for a specific element. (useful for deletion)

		size_t m_size = 0;
		size_t m_capcity = 0;

	};
}