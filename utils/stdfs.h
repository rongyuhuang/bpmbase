#pragma once

#if defined(_WIN32)
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

namespace stdfs = std::experimental::filesystem;
