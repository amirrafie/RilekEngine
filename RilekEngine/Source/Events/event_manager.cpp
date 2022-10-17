/******************************************************************************/
/*!
\file event_manager.cpp
\author Amir Rafie
\par email: amirrafiebin.m\@digipen.edu
\par DigiPen login: amirrafiebin.m
*/
/******************************************************************************/
#include "stdafx.h"
#include "event_manager.h"

Rilek::event_listener::~event_listener()
{
	for (std::function<void()>& fn : m_remove_functions)
	{
		fn();
	}
}
