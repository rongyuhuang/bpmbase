#pragma once

#include <stdint.h>

#include <ctime>
#include <string>

// 格式同:strftime,"%Y-%m-%d %H:%M:%S"
// 参考c++11：std::get_time std::put_time
// 参考c: strptime strftime
std::time_t bpm_str2ctime(const char* str_time, const char* format);
std::string bpm_ctime2str(std::time_t datetime, const char* format);


// 当天的时间，多少天
int bpm_daysofctime(int y, int m, int d);
int bpm_daysofctime(std::time_t t);

// 当天的时间，多少秒
int bpm_timeofctime(std::time_t t);
int bpm_timeofctime(const char* buf, const char* fmt);

// 同一天
// 周五夜盘和下周一合并
bool bpm_sametradingday(int64_t b1, int64_t b2);

void bpm_getLocalTime(char* currentTime,const char* fmt="%Y%m%d-%X");
