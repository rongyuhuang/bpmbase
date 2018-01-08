#ifndef KBARCONVERT_H
#define KBARCONVERT_H

#include <string>

namespace KBarConvert {
void convert(const std::string& fromPath,
    const std::string& toPath,
    int fromPeriod,
    int toPeriod);
}

#endif // KBARCONVERT_H
