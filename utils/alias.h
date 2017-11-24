#pragma once

namespace utils {

// Make the optimizer think that var is aliased. This is to prevent it from
// optimizing out variables that that would not otherwise be live at the point
// of a potential crash.
void Alias(const void* var);

} // namespace debug

