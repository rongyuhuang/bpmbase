#pragma once

#include "utils/stdfs.h"
#include <ctime>

namespace utils
{
    std::tm currentDateTime();

    stdfs::path currentProcessPath();
}
