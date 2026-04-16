#pragma once

#ifdef ODC_PLATFORM_WINDOWS
#ifndef NOMINMAX
// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOMINMAX
#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Odysseus/Core/Log.h"
#include "Odysseus/Core/Assert.h"

#include "Odysseus/Debug/Instrumentor.h"

#ifdef ODC_PLATFORM_WINDOWS
#include <Windows.h>
#endif // ODC_PLATFORM_WINDOWS
