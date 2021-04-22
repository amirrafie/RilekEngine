#pragma once
#include <iostream>

#ifdef _DEBUG
#define DEBUG_LOG(...) std::cout << __VA_ARGS__ << std::endl;
#else
#define DEBUG_LOG(...)
#endif