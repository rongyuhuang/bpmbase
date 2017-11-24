#include "alias.h"

namespace utils {

#if defined(_WIN32)
#pragma optimize("", off)
#endif

void Alias(const void* var)
{
}

#if defined(_WIN32)
#pragma optimize("", on)
#endif

} // namespace debug
