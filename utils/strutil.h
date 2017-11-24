#pragma once

#include <stdarg.h>

#include <algorithm>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<std::string> StringVector;

class StrUtil {
public:
    static void trimAll(std::string& str, char ch)
    {
        std::string::iterator destEnd = std::remove_if(
            str.begin(), str.end(), std::bind1st(std::equal_to<char>(), ch));
        str.resize(destEnd - str.begin());
    }

    static StringVector split(const std::string& str,
        const std::string& delims = "\t\n ",
        unsigned int maxSplits = 0)
    {
        StringVector ret;
        unsigned int numSplits = 0;

        // Use STL methods
        size_t start, pos;
        start = 0;
        do {
            pos = str.find_first_of(delims, start);
            if (pos == start) {
                // Do nothing
                start = pos + 1;
            } else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits)) {
                // Copy the rest of the std::string
                ret.push_back(str.substr(start));
                break;
            } else {
                // Copy up to delimiter
                ret.push_back(str.substr(start, pos - start));
                start = pos + 1;
            }
            // parse up to next real data
            start = str.find_first_not_of(delims, start);
            ++numSplits;

        } while (pos != std::string::npos);
        return ret;
    }

    static std::string printf(const char* pszFormat, ...)
    {
        va_list argptr;
        va_start(argptr, pszFormat);
        std::string result = printf(pszFormat, argptr);
        va_end(argptr);
        return result;
    }


    static const std::string BLANK()
    {
        static const std::string temp = std::string("");
        return temp;
    }

    static std::string printf(const char* pszFormat, va_list argptr)
    {
        int size = 1024;
        int len = 0;
        std::string ret;
        for (;;) {
            ret.resize(size + 1, 0);
            char* buf = (char*)ret.c_str();
            if (!buf) {
                return BLANK();
            }

            va_list argptrcopy;
            VaCopy(argptrcopy, argptr);

            len = _vsnprintf(buf, size, pszFormat, argptrcopy);
            va_end(argptrcopy);

            if (len >= 0 && len <= size) {
                // ok, there was enough space
                break;
            }
            size *= 2;
        }
        ret.resize(len);
        return ret;
    }

    static double parseDouble(const std::string& val)
    {
        return atof(val.c_str());
    }


    static int parseInt(const std::string& val)
    {
        return atoi(val.c_str());
    }

protected:
    static inline void VaCopy(va_list& dest, va_list& src) { dest = src; }
};
