#pragma once
#include "Tools/Containers/sparse_set.h"
#include "Tools/Delegate/delegate_container.h"
#include "entity.h"

namespace Rilek::ECS
{
	//forward declaration
	class world;

	struct testcomp
	{
		int a;
		int b;

		bool operator==(const testcomp& rhs)
		{
			return a == rhs.a && b == rhs.b;
		}
	};

	template<typename ComponentType>
	class component_container : public sparse_set<ComponentType>
	{
	public:
		//////////////////////
		// DELEGATES
		//////////////////////
		template<auto Fn>
		auto add_on_construct_delegate()
		{
			return m_on_construct_delegates.add_delegate<Fn>();
		}

		template<auto Fn, typename T>
		auto add_on_construct_delegate(T* t_instance)
		{
			return m_on_construct_delegates.add_delegate<Fn>(t_instance);
		}

		template<auto Fn>
		auto add_on_update_delegate()
		{
			return m_on_update_delegates.add_delegate<Fn>();
		}

		template<auto Fn, typename T>
		auto add_on_update_delegate(T* t_instance)
		{
			return m_on_update_delegates.add_delegate<Fn>(t_instance);
		}

		template<auto Fn>
		auto add_on_destruct_delegate()
		{
			return m_on_destroy_delegates.add_delegate<Fn>();
		}

		template<auto Fn, typename T>
		auto add_on_destruct_delegate(T* t_instance)
		{
			return m_on_destroy_delegates.add_delegate<Fn>(t_instance);
		}

		//////////////////////
		// MODIFIERS
		//////////////////////
		template<typename... TArgs>
		ComponentType& emplace(world& t_world, entity t_new_entity, TArgs&&... t_args)
		{
			if (sparse_set<ComponentType>::contains(t_new_entity))
			{
				RLK_ERROR("Entity {0} trying to emplace component that it already has!", t_new_entity);
				return sparse_set<ComponentType>::at(t_new_entity);
			}
			ComponentType& component = sparse_set<ComponentType>::emplace(t_new_entity, std::forward<TArgs>(t_args)...);
			m_on_construct_delegates.call_delegates(t_world, t_new_entity, component);
			return component;
		}

		void erase(world& t_world, entity t_entity)
		{
			if (sparse_set<ComponentType>::contains(t_entity))
			{
				m_on_destroy_delegates.call_delegates(t_world, t_entity, sparse_set<ComponentType>::at(t_entity));
				sparse_set<ComponentType>::erase(t_entity);
			}
		}

		// Reccomended to be used on a comp[letely new entity
		ComponentType& clone(world& t_world, entity t_dest_entity, entity t_source_entity)
		{
			if (sparse_set<ComponentType>::contains(t_source_entity))
			{
				if (!sparse_set<ComponentType>::contains(t_dest_entity))
				{
					ComponentType& component = sparse_set<ComponentType>::insert(t_dest_entity, sparse_set<ComponentType>::at(t_source_entity));
					m_on_construct_delegates.call_delegates(t_world, t_dest_entity, component);
					return component;
				}
				else
				{
					RLK_WARN("Entity {0} already existed before cloning!", t_dest_entity);
					ComponentType& component = sparse_set<ComponentType>::at(t_dest_entity);
					component = sparse_set<ComponentType>::at(t_source_entity);
					m_on_update_delegates.call_delegates(t_world, t_dest_entity, component);
					return component;
				}
			}
		}

		//////////////////////
		// LOOKUP
		//////////////////////
		// find based on component value
		entity Find(const ComponentType& component)
		{
			for (size_t index = 0; index < sparse_set<ComponentType>::m_dense.size(); ++index)
			{
				if (sparse_set<ComponentType>::m_dense[index] == component)
				{
					return sparse_set<ComponentType>::m_sparse_indices[index];
				}
			}
			return null_entity;
		}

		//////////////////////
		// ACCESS
		//////////////////////
		// please use these access functions to update any components	
		template<typename UpdateFunction>
		void update(world& t_world, entity t_entity, UpdateFunction&& t_function)
		{
			if (!sparse_set<ComponentType>::contains(t_entity))
			{
				RLK_ERROR("Entity {0} trying to update component that it does not have!", t_entity);
			}
			ComponentType& component = sparse_set<ComponentType>::at(t_entity);

			if constexpr (std::is_invocable_v<UpdateFunction, ComponentType>)
			{
				t_function(component);
				m_on_update_delegates.call_delegates(t_world, t_entity, component);
			}
			else if constexpr (std::is_invocable_v<UpdateFunction, ComponentType, entity>)
			{
				t_function(component, t_entity);
				m_on_update_delegates.call_delegates(t_world, t_entity, component);
			}
			else if constexpr (std::is_invocable_v<UpdateFunction, entity, ComponentType>)
			{
				t_function(t_entity, component);
				m_on_update_delegates.call_delegates(t_world, t_entity, component);
			}
			else if constexpr (std::is_invocable_v<UpdateFunction, entity>)
			{
				t_function(t_entity);
				m_on_update_delegates.call_delegates(t_world, t_entity, component);
			}
		}

		template<typename UpdateFunction>
		void update_all(world& t_world, UpdateFunction&& t_function)
		{
			for (size_t index = 0; index < sparse_set<ComponentType>::m_dense.size(); ++index)
			{
				if constexpr (std::is_invocable_v<UpdateFunction, ComponentType>)
				{
					t_function(sparse_set<ComponentType>::m_dense[index]);
					m_on_update_delegates.call_delegates(t_world, sparse_set<ComponentType>::m_sparse_indices[index], sparse_set<ComponentType>::m_dense[index]);
				}
				else if constexpr (std::is_invocable_v<UpdateFunction, ComponentType, entity>)
				{
					t_function(sparse_set<ComponentType>::m_dense[index], sparse_set<ComponentType>::m_sparse_indices[index]);
					m_on_update_delegates.call_delegates(t_world, sparse_set<ComponentType>::m_sparse_indices[index], sparse_set<ComponentType>::m_dense[index]);
				}
				else if constexpr (std::is_invocable_v<UpdateFunction, entity, ComponentType>)
				{
					t_function(sparse_set<ComponentType>::m_sparse_indices[index], sparse_set<ComponentType>::m_dense[index]);
					m_on_update_delegates.call_delegates(t_world, sparse_set<ComponentType>::m_sparse_indices[index], sparse_set<ComponentType>::m_dense[index]);
				}
				else if constexpr (std::is_invocable_v<UpdateFunction, entity>)
				{
					t_function(t_entity);
					m_on_update_delegates.call_delegates(t_world, sparse_set<ComponentType>::m_sparse_indices[index], sparse_set<ComponentType>::m_dense[index]);
				}
			}
		}

	private:
		delegate_container<void(world&, entity&, ComponentType&)> m_on_construct_delegates;
		delegate_container<void(world&, entity&, ComponentType&)> m_on_update_delegates;
		delegate_container<void(world&, entity&, ComponentType&)> m_on_destroy_delegates;
	};
}