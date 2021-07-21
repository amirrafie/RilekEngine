#include "stdafx.h"
#include "world.h"

namespace Rilek::ECS
{
	entity world::create_entity()
	{
		// if there are entities to be recycled, use it
		if (m_recycable_entities.size())
		{
			entity new_entity = m_recycable_entities.back();
			m_recycable_entities.pop_back();
			m_entities.emplace(new_entity);
			return new_entity;
		}
		// otherwise, the biggest numbered entity used so far is the total number of entities we have
		else
		{
			entity new_entity = m_entities.size();
			m_entities.emplace(new_entity);
			return new_entity;
		}
	}

	entity world::create_clone(entity t_source_entity)
	{
		entity new_entity = create_entity();
		for (auto& component_container : m_component_containers)
		{
			component_container.m_clone_fn(component_container.m_container.get(), *this, new_entity, t_source_entity);
		}
		return new_entity;
	}

	bool world::contains_entity(entity t_entity)
	{
		return m_entities.find(t_entity) != m_entities.end();
	}

	void world::delete_entity(entity t_entity)
	{
		if (contains_entity(t_entity))
		{
			m_free_list.emplace(t_entity);
		}
	}

	void world::delete_all_entities()
	{
		m_to_clear = true;
	}
	void world::update()
	{
		// simple clear everything if marked for clear
		if (m_to_clear)
		{
			m_entities.clear();
			m_free_list.clear();
			for (auto& component_free_list : m_component_free_lists)
			{
				component_free_list.clear();
			}
			m_recycable_entities.clear();
			for (auto& component_container : m_component_containers)
			{
				component_container.m_clear_fn(component_container.m_container.get());
			}

			m_to_clear = false;
		}
		else
		{
			empty_component_free_list();
			empty_free_list();
		}
	}
	size_t world::size()
	{
		return m_entities.size();
	}

	void world::empty_component_free_list()
	{
		// for each component
		for (size_t component_index = 0 ; component_index < m_component_free_lists.size(); component_index++)
		{
			for (auto entity : m_component_free_lists[component_index])
			{
				//destroy the component of that entity
				m_component_containers[component_index].m_erase_fn(m_component_containers[component_index].m_container.get(), *this, entity);
			}
			m_component_free_lists[component_index].clear();
		}
	}

	void world::empty_free_list()
	{
		for (auto entity : m_free_list)
		{
			m_entities.erase(entity);

			//destroy components of that entity
			for (auto& container : m_component_containers)
			{
				container.m_erase_fn(container.m_container.get(), *this, entity);
			}

			m_recycable_entities.emplace_back(entity);
		}
		m_free_list.clear();
	}
}


