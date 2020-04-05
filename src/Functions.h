#pragma once

#include <functional>

namespace cbee
{

typedef std::function<void()> ConnectedFunc;
typedef std::function<void()> ReadDoneFunc;
typedef std::function<void()> WriteDoneFunc;
typedef std::function<void()> DisConnectedFunc;

} // namespace cbee
