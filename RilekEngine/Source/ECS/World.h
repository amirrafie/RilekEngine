#pragma once
#include <memory>
#include <vector>

#include "component_container.h"
namespace Rilek::ECS
{
	class world
	{
		using component_container_wrapper = std::unique_ptr<void, void(*)(void*)>;
		using component_ID = unsigned int;

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

	public:


		// Create instance of a component container for the component
		template<typename Component>
		void register_component()
		{
			// get the system id, and check if the id corresponds to the latest index that will be assigned
			component_ID id = get_component_ID<Component>();
			RLK_ASSERT(id == m_component_containers.size(), "Error registering component");

			//  Create the system
			component_container_wrapper component_container(new component_container<Component>{}, [](void* ptr) {delete static_cast<component_container<Component>*>(ptr); });
			auto& component_container_ref = m_component_containers.emplace_back(std::move(component_container));

			return;
		}
		
	private:
		std::vector<component_container_wrapper> m_component_containers;
	};
}