#pragma once
#ifndef SPARSE_SET_H
#define SPARSE_SET_H

namespace RILEK
{
	template<typename T>
	class sparse_set
	{
	public:

		void grow(size_t t_new_capacity)
		{
			if (t_new_capacity > m_capcity)
			{
				m_capcity = t_new_capacity;
				m_sparse.resize(m_capcity, empty_index);
				m_dense.reserve(m_capcity);
				m_sparse_indices.reserve(m_capcity);
			}
		}

		// create element in place in indexed position
		template<typename... TArgs>
		T& emplace(size_t t_index, TArgs&&... t_args)
		{
			if (t_index >= m_capcity)
			{
				grow(t_index + 1);
			}

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

		// copy element in place in indexed position
		T& insert(size_t t_index, const T& t_value)
		{
			if (t_index >= m_capcity)
			{
				grow(t_index + 1);
			}
			if (m_sparse[t_index] == empty_index)
			{
				m_sparse[t_index] = m_dense.size();
				m_dense.push_back(t_value);
				m_sparse_indices.emplace_back(t_index);
				++m_size;
			}
			else
			{
				// if element already in the container, just construct and throw away (following stl behaviour)
				T temp(t_value);
				(void)temp;
			}
			return m_dense[m_sparse[t_index]];
		}

		// access
		T& at(size_t t_index)
		{
			if (t_index >= m_capcity || m_sparse[t_index] == empty_index)
			{
				throw std::out_of_range("SparseSet out of range!");
			}
			return m_dense[m_sparse[t_index]];
		}

		// use vetor iterator for ranges
		auto begin()
		{
			return m_dense.begin();
		}

		auto end()
		{
			return m_dense.end();
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
	private:
		static const size_t empty_index = std::numeric_limits<size_t>::max();

		std::vector<size_t> m_sparse;
		std::vector<T> m_dense;
		std::vector<size_t> m_sparse_indices;	// to use to keep track of the index used for a specific element. (useful for deletion)

		size_t m_size = 0;
		size_t m_capcity = 0;

	};
}

#endif