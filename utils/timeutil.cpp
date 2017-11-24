#include "timeutil.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define TM_YEAR_BASE 1900

/*
 * We do not implement alternate representations. However, we always
 * check whether a given modifier is allowed for a certain conversion.
 */
#define ALT_E 0x01
#define ALT_O 0x02
#define LEGAL_ALT(x)           \
    {                          \
        if (alt_format & ~(x)) \
            return (0);        \
    }

static int conv_num(const char**, int*, int, int);

static const char* day[7] = { "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday" };
static const char* abday[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char* mon[12] = { "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December" };
static const char* abmon[12] = { "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec" };
static const char* am_pm[2] = { "AM", "PM" };

// window上自己实现strptime函数，linux已经提供strptime
// strptime函数windows平台上实现
static char* bpm_strptime(const char* buf, const char* fmt, struct tm* tm)
{
    char c;
    const char* bp;
    size_t len = 0;
    int alt_format, i, split_year = 0;

    bp = buf;

    while ((c = *fmt) != '\0') {
        /* Clear `alternate' modifier prior to new conversion. */
        alt_format = 0;

        /* Eat up white-space. */
        if (isspace(c)) {
            while (isspace(*bp))
                bp++;

            fmt++;
            continue;
        }

        if ((c = *fmt++) != '%')
            goto literal;

    again:
        switch (c = *fmt++) {
        case '%': /* "%%" is converted to "%". */
        literal:
            if (c != *bp++)
                return (0);
            break;

        /*
       * "Alternative" modifiers. Just set the appropriate flag
       * and start over again.
       */
        case 'E': /* "%E?" alternative conversion modifier. */
            LEGAL_ALT(0);
            alt_format |= ALT_E;
            goto again;

        case 'O': /* "%O?" alternative conversion modifier. */
            LEGAL_ALT(0);
            alt_format |= ALT_O;
            goto again;

        /*
       * "Complex" conversion rules, implemented through recursion.
       */
        case 'c': /* Date and time, using the locale's format. */
            LEGAL_ALT(ALT_E);
            if (!(bp = bpm_strptime(bp, "%x %X", tm)))
                return (0);
            break;

        case 'D': /* The date as "%m/%d/%y". */
            LEGAL_ALT(0);
            if (!(bp = bpm_strptime(bp, "%m/%d/%y", tm)))
                return (0);
            break;

        case 'R': /* The time as "%H:%M". */
            LEGAL_ALT(0);
            if (!(bp = bpm_strptime(bp, "%H:%M", tm)))
                return (0);
            break;

        case 'r': /* The time in 12-hour clock representation. */
            LEGAL_ALT(0);
            if (!(bp = bpm_strptime(bp, "%I:%M:%S %p", tm)))
                return (0);
            break;

        case 'T': /* The time as "%H:%M:%S". */
            LEGAL_ALT(0);
            if (!(bp = bpm_strptime(bp, "%H:%M:%S", tm)))
                return (0);
            break;

        case 'X': /* The time, using the locale's format. */
            LEGAL_ALT(ALT_E);
            if (!(bp = bpm_strptime(bp, "%H:%M:%S", tm)))
                return (0);
            break;

        case 'x': /* The date, using the locale's format. */
            LEGAL_ALT(ALT_E);
            if (!(bp = bpm_strptime(bp, "%m/%d/%y", tm)))
                return (0);
            break;

        /*
       * "Elementary" conversion rules.
       */
        case 'A': /* The day of week, using the locale's form. */
        case 'a':
            LEGAL_ALT(0);
            for (i = 0; i < 7; i++) {
                /* Full name. */
                len = strlen(day[i]);
                if (strncmp(day[i], bp, len) == 0)
                    break;

                /* Abbreviated name. */
                len = strlen(abday[i]);
                if (strncmp(abday[i], bp, len) == 0)
                    break;
            }

            /* Nothing matched. */
            if (i == 7)
                return (0);

            tm->tm_wday = i;
            bp += len;
            break;

        case 'B': /* The month, using the locale's form. */
        case 'b':
        case 'h':
            LEGAL_ALT(0);
            for (i = 0; i < 12; i++) {
                /* Full name. */
                len = strlen(mon[i]);
                if (strncmp(mon[i], bp, len) == 0)
                    break;

                /* Abbreviated name. */
                len = strlen(abmon[i]);
                if (strncmp(abmon[i], bp, len) == 0)
                    break;
            }

            /* Nothing matched. */
            if (i == 12)
                return (0);

            tm->tm_mon = i;
            bp += len;
            break;

        case 'C': /* The century number. */
            LEGAL_ALT(ALT_E);
            if (!(conv_num(&bp, &i, 0, 99)))
                return (0);

            if (split_year) {
                tm->tm_year = (tm->tm_year % 100) + (i * 100);
            } else {
                tm->tm_year = i * 100;
                split_year = 1;
            }
            break;

        case 'd': /* The day of month. */
        case 'e':
            LEGAL_ALT(ALT_O);
            if (!(conv_num(&bp, &tm->tm_mday, 1, 31)))
                return (0);
            break;

        case 'k': /* The hour (24-hour clock representation). */
            LEGAL_ALT(0);
        /* FALLTHROUGH */
        case 'H':
            LEGAL_ALT(ALT_O);
            if (!(conv_num(&bp, &tm->tm_hour, 0, 23)))
                return (0);
            break;

        case 'l': /* The hour (12-hour clock representation). */
            LEGAL_ALT(0);
        /* FALLTHROUGH */
        case 'I':
            LEGAL_ALT(ALT_O);
            if (!(conv_num(&bp, &tm->tm_hour, 1, 12)))
                return (0);
            if (tm->tm_hour == 12)
                tm->tm_hour = 0;
            break;

        case 'j': /* The day of year. */
            LEGAL_ALT(0);
            if (!(conv_num(&bp, &i, 1, 366)))
                return (0);
            tm->tm_yday = i - 1;
            break;

        case 'M': /* The minute. */
            LEGAL_ALT(ALT_O);
            if (!(conv_num(&bp, &tm->tm_min, 0, 59)))
                return (0);
            break;

        case 'm': /* The month. */
            LEGAL_ALT(ALT_O);
            if (!(conv_num(&bp, &i, 1, 12)))
                return (0);
            tm->tm_mon = i - 1;
            break;

        case 'p': /* The locale's equivalent of AM/PM. */
            LEGAL_ALT(0);
            /* AM? */
            if (strcmp(am_pm[0], bp) == 0) {
                if (tm->tm_hour > 11)
                    return (0);

                bp += strlen(am_pm[0]);
                break;
            }
            /* PM? */
            else if (strcmp(am_pm[1], bp) == 0) {
                if (tm->tm_hour > 11)
                    return (0);

                tm->tm_hour += 12;
                bp += strlen(am_pm[1]);
                break;
            }

            /* Nothing matched. */
            return (0);

        case 'S': /* The seconds. */
            LEGAL_ALT(ALT_O);
            if (!(conv_num(&bp, &tm->tm_sec, 0, 61)))
                return (0);
            break;

        case 'U': /* The week of year, beginning on sunday. */
        case 'W': /* The week of year, beginning on monday. */
            LEGAL_ALT(ALT_O);
            /*
         * XXX This is bogus, as we can not assume any valid
         * information present in the tm structure at this
         * point to calculate a real value, so just check the
         * range for now.
         */
            if (!(conv_num(&bp, &i, 0, 53)))
                return (0);
            break;

        case 'w': /* The day of week, beginning on sunday. */
            LEGAL_ALT(ALT_O);
            if (!(conv_num(&bp, &tm->tm_wday, 0, 6)))
                return (0);
            break;

        case 'Y': /* The year. */
            LEGAL_ALT(ALT_E);
            if (!(conv_num(&bp, &i, 0, 9999)))
                return (0);

            tm->tm_year = i - TM_YEAR_BASE;
            break;

        case 'y': /* The year within 100 years of the epoch. */
            LEGAL_ALT(ALT_E | ALT_O);
            if (!(conv_num(&bp, &i, 0, 99)))
                return (0);

            if (split_year) {
                tm->tm_year = ((tm->tm_year / 100) * 100) + i;
                break;
            }
            split_year = 1;
            if (i <= 68)
                tm->tm_year = i + 2000 - TM_YEAR_BASE;
            else
                tm->tm_year = i + 1900 - TM_YEAR_BASE;
            break;

        /*
       * Miscellaneous conversions.
       */
        case 'n': /* Any kind of white-space. */
        case 't':
            LEGAL_ALT(0);
            while (isspace(*bp))
                bp++;
            break;

        default: /* Unknown/unsupported conversion. */
            return (0);
        }
    }

    /* LINTED functional specification */
    return ((char*)bp);
}

static int conv_num(const char** buf, int* dest, int llim, int ulim)
{
    int result = 0;

    /* The limit also determines the number of valid digits. */
    int rulim = ulim;

    if (**buf < '0' || **buf > '9')
        return (0);

    do {
        result *= 10;
        result += *(*buf)++ - '0';
        rulim /= 10;
    } while ((result * 10 <= ulim) && rulim && **buf >= '0' && **buf <= '9');

    if (result < llim || result > ulim)
        return (0);

    *dest = result;
    return (1);
}

time_t bpm_str2ctime(const char* str_time, const char* format)
{
    tm temp_tm;
    time_t temp_time;
    bpm_strptime(str_time, format, &temp_tm);
    temp_tm.tm_isdst = -1;
    temp_time = mktime(&temp_tm);
    return temp_time;
}

std::string bpm_ctime2str(time_t datetime, const char* format)
{
    struct tm temp_time;
    localtime_s(&temp_time, &datetime);
    char str_time[64]={0};
    strftime(str_time, 64, format, &temp_time);
    return str_time;
}

int bpm_daysofctime(int y, int m, int d)
{
    int month[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, i, s = 0;
    for (i = 1900; i < y; i++) {
        s += 365;
        if (y % 400 == 0 || y % 4 == 0 && y % 100)
            ++s;
    }
    for (i = 1; i < m; i++)
        s += month[i];
    return s + d - 1;
}

int bpm_daysofctime(std::time_t t)
{
    std::tm m;
    localtime_s(&m, &t);
    return bpm_daysofctime(m.tm_year + 1900, m.tm_mon + 1, m.tm_mday);
}

bool bpm_sametradingday(int64_t b1, int64_t b2)
{
    std::time_t t1 = (std::time_t)b1 + 6 * 60 * 60;
    std::time_t t2 = (std::time_t)b2 + 6 * 60 * 60;
    std::tm m1;
    localtime_s(&m1, &t1);
    std::tm m2;
    localtime_s(&m2, &t2);
    int d1 = bpm_daysofctime(m1.tm_year + 1900, m1.tm_mon + 1, m1.tm_mday);
    int d2 = bpm_daysofctime(m2.tm_year + 1900, m2.tm_mon + 1, m2.tm_mday);
    if (d1 == d2) {
        return true;
    }
    if (m1.tm_wday == 6 && m2.tm_wday == 1) {
        if (std::abs(d2 - d1) == 2) {
            return true;
        }
    }

    return false;
}

int bpm_timeofctime(std::time_t t)
{
    std::tm m;
    localtime_s(&m, &t);
    int s = m.tm_hour * 60 * 60 + m.tm_min * 60 + m.tm_sec;
    return s;
}

int bpm_timeofctime(const char* buf, const char* fmt)
{
    std::time_t t = bpm_str2ctime(buf, fmt);
    return bpm_timeofctime(t);
}
