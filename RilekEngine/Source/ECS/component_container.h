#pragma once
#include "Tools/Containers/sparse_set.h"
#include "Tools/Delegate/delegate_container.h"
#include "entity.h"
namespace Rilek::ECS
{
	//forward declaration
	class world;

	template<typename ComponentType>
	class component_container : public sparse_set<ComponentType>
	{
	public:
		template<auto Fn>
		delegate_id add_construct_delegate()
		{

		}
	private:
		delagate_container<void(world&, entity&, ComponentType&)> m_on_construct_delegates;
		delagate_container<void(world&, entity&, ComponentType&)> m_on_update_delegates;
		delagate_container<void(world&, entity&, ComponentType&)> m_on_destroy_delegates;
	};
}