#include "kbarconvert.h"
#include "kbarutils/kbartable.h"
#include "utils/strutil.h"
#include "utils/timeutil.h"
#include "utils/logging.h"

#include <fstream>

namespace KBarConvert {

void convert(const std::string& fromPath,
    const std::string& toPath,
    int fromPeriod,
    int toPeriod)
{
    KBarTable table(toPeriod, 10000, fromPeriod);

    if (1) {
        std::ifstream file;
        file.open(fromPath);
        std::string line;
        std::vector<std::string> items;
        KBarData bar;
        while (file.is_open() && std::getline(file, line)) {
            items.clear();
            items = StrUtil::split(line, " ");
            CHECK(items.size() >= 9);
            strncpy(bar.symbol, "symbol", sizeof(bar.symbol) - 1);
            bar.actionDatetime =bpm_str2ctime(items[0].c_str(), "%Y%m%d-%H:%M:%S");

            bar.openPrice = StrUtil::parseDouble(items[3]);
            bar.highPrice = StrUtil::parseDouble(items[4]);
            bar.lowPrice = StrUtil::parseDouble(items[5]);
            bar.closePrice = StrUtil::parseDouble(items[6]);

            bar.volume = StrUtil::parseDouble(items[7]);
            // bar.totalVolume = 1;
            bar.openInterest = StrUtil::parseDouble(items[8]);

            table.appendSubKBar(&bar);
        }

        table.finish();
        file.close();
    }

    if (1) {
        std::ofstream file;
        file.open(toPath);
        KBarTableSnapshot snapshot;
        table.getSnapshot(snapshot);
        for (int i = 0; i < snapshot.actionDatetime.len; i++) {
            if (file.is_open()) {
                std::string dt = bpm_ctime2str(snapshot.actionDatetime[i], "%Y%m%d-%H:%M:%S");
                file << dt;
                file << " ";
                file << "exchanege";
                file << " ";
                file << "symbol";
                file << " ";
                file << snapshot.openPrice[i];
                file << " ";
                file << snapshot.highPrice[i];
                file << " ";
                file << snapshot.lowPrice[i];
                file << " ";
                file << snapshot.closePrice[i];
                file << " ";
                file << snapshot.volume[i];
                file << " ";
                file << snapshot.openInterest[i];
                file << std::endl;
            }
        }
        file.flush();
        file.close();
    }
}

} // namespace KBarConvert
