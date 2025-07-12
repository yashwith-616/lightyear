#pragma once

#include "Lightyear/Common/Macros.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <execution>
#include <filesystem>
#include <fstream>
#include <functional>
#include <queue>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <shared_mutex>
#include <thread>

#include <memory>
#include <new>
#include <utility>

LY_DISABLE_WARNINGS_PUSH
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
LY_DISABLE_WARNINGS_POP