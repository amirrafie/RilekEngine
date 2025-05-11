#pragma once
#include <memory>
#include <unordered_set>

#include "Tools/Containers/vector.h"
#include "component_container.h"

#define REGISTER_COMPONENT_TO_WORLD(world, c) world.register_component<c>(#c)

namespace Rilek::ECS
{
	class world
	{
		using component_ID = unsigned int;
		// function pointers so that can access them easily while iterating through all component containers
		struct component_container_wrapper
		{
			component_container_wrapper(const char* t_name, void* t_container_ptr, void(*t_deleter)(void*), void(*t_erase_fn)(void*, world&, entity), void(*t_clone_fn)(void*, world&, entity, entity), void(*t_clear_fn)(void*)) :
				m_component_name(t_name),
				m_container{ t_container_ptr,  t_deleter },
				m_erase_fn{ t_erase_fn },
				m_clone_fn{ t_clone_fn },
				m_clear_fn{ t_clear_fn }
			{}

			const char* m_component_name;
			std::unique_ptr<void, void(*)(void*)> m_container;
			void(*m_erase_fn)(void*, world&, entity);
			void(*m_clone_fn)(void*, world&, entity, entity);
			void(*m_clear_fn)(void*);
		};



	public:
		/////////////////
		// COMPONENTS
		/////////////////

		// Create instance of a component container for the component
		template<typename Component>
		void register_component(const char* name)
		{
			// get the system id, and check if the id corresponds to the latest index that will be assigned
			component_ID id = get_component_ID<Component>();
			RLK_ASSERT(id == m_component_containers.size(), "Error registering component");

			//  Create the component container and function pointers
			m_component_containers.emplace_back
			(
				name,
				new Rilek::ECS::component_container<Component>{},
				[](void* ptr) {delete static_cast<Rilek::ECS::component_container<Component>*>(ptr); },
				[](void* t_component_container, world& t_world, entity t_entity) {(*static_cast<Rilek::ECS::component_container<Component>*>(t_component_container)).Rilek::ECS::component_container<Component>::erase(t_world, t_entity); },
				[](void* t_component_container, world& t_world, entity t_dest_entity, entity t_source_entity) {(*static_cast<Rilek::ECS::component_container<Component>*>(t_component_container)).Rilek::ECS::component_container<Component>::clone(t_world, t_dest_entity, t_source_entity); },
				[](void* t_component_container) {(*static_cast<Rilek::ECS::component_container<Component>*>(t_component_container)).Rilek::ECS::component_container<Component>::clear(); }
			);
			m_component_free_lists.emplace_back();
			return;
		}

		/////////////////
		// ENTITIES
		/////////////////
		entity create_entity();
		entity create_clone(entity t_source_entity);

		bool contains_entity(entity t_entity);

		void delete_entity(entity t_entity);
		void delete_all_entities();

		void update();

		size_t size();

		//////////////////////////
		// COMPONENTS - DELEGATES
		//////////////////////////
		template<typename Component, auto Fn>
		auto add_on_construct_delegate() { return get_component_container<Component>().add_on_construct_delegate<Fn>(); }
		template<typename Component, auto Fn, typename T>
		auto add_on_construct_delegate(T* t_instance) { return get_component_container<Component>().add_on_construct_delegate<Fn>(t_instance); }
		template<typename Component, auto Fn>
		auto add_on_update_delegate() { return get_component_container<Component>().add_on_update_delegate<Fn>(); }
		template<typename Component, auto Fn, typename T>
		auto add_on_update_delegate(T* t_instance) { return get_component_container<Component>().add_on_update_delegate<Fn>(t_instance); }
		template<typename Component, auto Fn>
		auto add_on_destruct_delegate() { return get_component_container<Component>().add_on_destruct_delegate<Fn>(); }
		template<typename Component, auto Fn, typename T>
		auto add_on_destruct_delegate(T* t_instance) { return get_component_container<Component>().add_on_destruct_delegate<Fn>(t_instance); }

		//////////////////////////
		// COMPONENTS - MODIFIERS
		//////////////////////////
		template<typename Component, typename... TArgs>
		Component& add_component(entity t_entity, TArgs&&... t_args){ return get_component_container<Component>().emplace(*this, t_entity, std::forward<TArgs>(t_args)...); }
		//erase works differently, will use freelist
		template<typename Component>
		void remove_component(entity t_entity) { component_ID id = get_component_ID<Component>(); RLK_ASSERT((id < m_component_containers.size()), "Compnonent was not registered!"); m_component_free_lists[id].emplace(t_entity); };

		//////////////////////////
		// COMPONENTS - LOOKUP
		//////////////////////////
		template<typename Component>
		entity find_entity(const Component& component) { return get_component_container<Component>().find(component); }

		//////////////////////////
		// COMPONENTS - ACCESS
		//////////////////////////
		template<typename Component, typename UpdateFunction>
		void update_component(entity t_entity, UpdateFunction&& t_function) { get_component_container<Component>().update<UpdateFunction>(*this, t_entity, std::forward<UpdateFunction>(t_function)); }
		template<typename Component, typename UpdateFunction>
		void update_component_all(UpdateFunction&& t_function) { get_component_container<Component>().update_all<UpdateFunction>(*this, std::forward<UpdateFunction>(t_function)); }

	private:
		component_ID generate_component_ID()
		{
			static component_ID component_ID = 0;
			return component_ID++;
		}

		template<typename Component>
		component_ID get_component_ID()
		{
			static component_ID ID = generate_component_ID();
			return ID;
		}

		// Get the component container responsible for a specific component
		template<typename Component>
		component_container<Component>& get_component_container()
		{
			component_ID id = get_component_ID<Component>();
			RLK_ASSERT((id < m_component_containers.size()), "Compnonent was not registered!");
			return *static_cast<component_container<Component>*>(m_component_containers[id].m_container.get());
		}

		void empty_component_free_list();
		void empty_free_list();

		////////////////////
		// Member variables
		////////////////////

		// used to keep track of exiting entities, dont actually have to care about the value
		std::unordered_set<entity> m_entities;
		// list of entities to delete
		std::unordered_set<entity> m_free_list;
		vector<std::unordered_set<entity>> m_component_free_lists;
		// list of recycable entites when creating new entities
		vector<entity> m_recycable_entities;
		// component containers
		vector<component_container_wrapper> m_component_containers;
		// flags
		bool m_to_clear = false;
	};
}