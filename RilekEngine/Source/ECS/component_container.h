#pragma once
#include "Tools/Containers/sparse_set.h"
namespace RILEK::ECS
{
	template<typename ComponentType>
	class component_container : public sparse_set<ComponentType>
	{
	public:

	private:
	};
}