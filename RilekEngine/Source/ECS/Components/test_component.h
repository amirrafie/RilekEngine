#pragma once
struct test_component
{
	int m_value;

	bool operator==(const test_component& rhs) { return m_value == rhs.m_value; }
};