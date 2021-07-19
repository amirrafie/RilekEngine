#pragma once
namespace Rilek::ECS
{
	using entity = size_t;
	static constexpr entity null_entity = std::numeric_limits<entity>::max();
}