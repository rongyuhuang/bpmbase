#include "utils/system.h"

#include "utils/logging.h"

#if defined(_WIN32)
#include <windows.h>
#endif

namespace utils
{
    std::tm currentDateTime()
    {
        using std::chrono::system_clock;
        std::time_t now_time = system_clock::to_time_t(system_clock::now());
        std::tm parts;
#if defined(_WIN32)
        localtime_s(&parts, &now_time);
#else
        parts = *localtime(&now_time);
#endif
        return parts;
    }

    stdfs::path currentProcessPath()
    {
#if defined(_WIN32)
        wchar_t buf[_MAX_PATH];
        const int bytes = GetModuleFileNameW(nullptr, buf, _MAX_PATH);
        if (bytes == 0) std::abort();
        return stdfs::path(buf, buf + bytes);
#else
        std::array<char, 1024 * 4> buf;
        auto written = readlink("/proc/self/exe", buf.data(), buf.size());
        CHECK(written != -1);
        return stdfs::path(buf.data(), buf.data() + written);
#endif
    }

}
