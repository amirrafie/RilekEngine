#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <Windows.h>
#include <memory>
#include <functional>
#include <cassert>
#include <vector>
#include <chrono>

#include "Tools/Logger/logger.h"
#include "Tools/Delegate/delegate.h"
#include "Tools/Delegate/delegate_container.h"