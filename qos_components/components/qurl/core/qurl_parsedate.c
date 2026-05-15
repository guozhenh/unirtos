/*
 * Portions of this file are derived from curl/libcurl's lib/parsedate.c.
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * should be included in the curl distribution. The terms are also
 * available at https://curl.se/docs/copyright.html.
 *
 * For qurl redistribution, see:
 * - qos_components/components/qurl/THIRD_PARTY_NOTICES.md
 * - qos_components/components/qurl/licenses/curl.txt
 */
/** @file         qurl_parsedate.c
 *  @brief        Brief Description
 *  @details      Detailed description: Refer to curl
 *  @author       lzm
 *  @date         2024-01-03 16:23:33
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

/**
 * The following are the date string formats supported by this parser:

The format defined in RFC 2616 3.3.1, such as: Sun, 06 Nov 1994 08:49:37 GMT
Supports date formats without the day of the week, such as: 06 Nov 1994 08:49:37 GMT
Supports date formats with omitted time zones, such as: 06 Nov 1994 08:49:37
Supports date formats in different orders, such as: 1994 Nov 6 08:49:37
Supports omission of the time part in dates, e.g.: 1994 Nov 6
Supports date formats with different separators, such as: 1994.Nov.6
Supports common time zone names, such as: Sun, 06 Nov 1994 08:49:37 CET
Supports time zones specified in RFC822 format, such as: Sun, 12 Sep 2004 15:05:58 -0700
Supports compact numeric date strings, such as: 20040912 15:05:58 -0700

*/

#include "qurl_config.h"
#include "qurl_code.h"
#include "qurl_def_in.h"
#include "qurl_log.h"
#include "qurl_ctype.h"

#define PD_NAME_LEN (12)

typedef enum
{
    QURL_PD_ECODE_OK = 0, /*!< Parsing successful*/
    QURL_PD_ECODE_FAIL,   /*!< Parsing failed*/
    QURL_PD_ECODE_LATER,  /*!< Overflow, meaning the parsed time is outside the upper limit range*/
    QURL_PD_ECODE_SOONER, /*!< Underflow*/
} qurl_pd_ecode_e;

typedef enum
{
    QURL_PD_ASSUNE_DATE_MDAY = 0, /*!< Assuming the date string is missing the day and year parts, only the month and time parts are present.*/
    QURL_PD_ASSUNE_DATE_YEAR,     /*!< Assuming the date string is missing the year part, containing only the month, day, and time components.*/
    QURL_PD_ASSUNE_DATE_TIME,     /*!< Assume the date string contains only the time part, without the date part.*/
} qurl_pd_assume_e;

#if QURL_CFG_ENABLE_PARSEDATE

const char *const qurl_pd_wkday[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
const char *const qurl_pd_month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

static const char *const qurl_pd_weekday[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

/*tz: time zone: time zone*/
struct pd_tz_info
{
    char name[5];
    int  offset; /*+/- minutes*/
};

/*Here is a set of commonly used time zone names.*/
#define QURL_PD_DAYZONE -60         /*Daylight Saving Time offset correction (DST is 60 minutes ahead)*/
static const struct pd_tz_info tz[] = {
    {"GMT", 0},                     /*Greenwich Mean Time*/
    {"UT", 0},                      /*Universal Time: Coordinated Universal Time*/
    {"UTC", 0},                     /*Universal (Coordinated): Coordinated Universal Time*/
    {"WET", 0},                     /*Western European: Western European Time*/
    {"BST", 0 QURL_PD_DAYZONE},     /*British Summer Time*/
    {"WAT", 60},                    /*West Africa: West Africa Time*/
    {"AST", 240},                   /*Atlantic Standard: Atlantic Standard Time*/
    {"ADT", 240 QURL_PD_DAYZONE},   /*Atlantic Daylight Time*/
    {"EST", 300},                   /*Eastern Standard: Eastern Standard Time*/
    {"EDT", 300 QURL_PD_DAYZONE},   /*Eastern Daylight: Eastern Daylight Time (EDT)*/
    {"CST", 360},                   /*Central Standard: Central Standard Time (CST)*/
    {"CDT", 360 QURL_PD_DAYZONE},   /*Central Daylight Time: Central Daylight Time in the United States*/
    {"MST", 420},                   /*Mountain Standard: Mountain Standard Time (MST)*/
    {"MDT", 420 QURL_PD_DAYZONE},   /*Mountain Daylight Time: US Mountain Daylight Time*/
    {"PST", 480},                   /*Pacific Standard: Pacific Standard Time in the United States*/
    {"PDT", 480 QURL_PD_DAYZONE},   /*Pacific Daylight Time: US Pacific Daylight Time*/
    {"YST", 540},                   /*Yukon Standard: Yukon Standard Time*/
    {"YDT", 540 QURL_PD_DAYZONE},   /*Yukon Daylight: Yukon Daylight Time*/
    {"HST", 600},                   /*Hawaii Standard: Hawaii Standard Time*/
    {"HDT", 600 QURL_PD_DAYZONE},   /*Hawaii Daylight: Hawaii Daylight Time*/
    {"CAT", 600},                   /*Central Alaska: Alaska Central Standard Time*/
    {"AHST", 600},                  /*Alaska-Hawaii Standard Time*/
    {"NT", 660},                    /*Name: Alaska Nome Standard Time*/
    {"IDLW", 720},                  /*International Date Line West*/
    {"CET", -60},                   /*Central European: Central European Standard Time*/
    {"MET", -60},                   /*Middle European: Central European Standard Time*/
    {"MEWT", -60},                  /*Middle European Winter: Central European Winter Time*/
    {"MEST", -60 QURL_PD_DAYZONE},  /*Middle European Summer Time*/
    {"CEST", -60 QURL_PD_DAYZONE},  /*Central European Summer Time*/
    {"MESZ", -60 QURL_PD_DAYZONE},  /*Middle European Summer Time*/
    {"FWT", -60},                   /*French Winter: French Winter Time*/
    {"FST", -60 QURL_PD_DAYZONE},   /*French Summer: French Summer Time*/
    {"EET", -120},                  /*Eastern Europe, USSR Zone 1: Eastern European Standard Time, Soviet Union Zone 1*/
    {"WAST", -420},                 /*West Australian Standard Time*/
    {"WADT", -420 QURL_PD_DAYZONE}, /*West Australian Daylight Time*/
    {"CCT", -480},                  /*China Coast, USSR Zone 7: China Coast, USSR Zone 7*/
    {"JST", -540},                  /*Japan Standard, USSR Zone 8: Japan Standard Time, USSR Zone 8*/
    {"EAST", -600},                 /*Eastern Australian Standard Time*/
    {"EADT", -600 QURL_PD_DAYZONE}, /*Eastern Australian Daylight Time*/
    {"GST", -600},                  /*Guam Standard, USSR Zone 9: Guam Standard Time, USSR Zone 9*/
    {"NZT", -720},                  /*New Zealand: New Zealand Standard Time*/
    {"NZST", -720},                 /*New Zealand Standard Time*/
    {"NZDT", -720 QURL_PD_DAYZONE}, /*New Zealand Daylight Time*/
    {"IDLE", -720},                 /*International Date Line East*/
    /*Next are the military time zone names. RFC822 allows the use of these names, but (as mentioned in RFC 1123) their notation is incorrect. Here, we use the correct notation to match actual military usage.*/
    {"A", 1 * 60},   /*Alpha: Alpha Time Zone*/
    {"B", 2 * 60},   /*Bravo: Bravo Time Zone*/
    {"C", 3 * 60},   /*Charlie: Charlie Time Zone*/
    {"D", 4 * 60},   /*Delta: Delta Time Zone*/
    {"E", 5 * 60},   /*Echo: Echo Time Zone*/
    {"F", 6 * 60},   /*Foxtrot: Foxtrot Time Zone*/
    {"G", 7 * 60},   /*Golf: Golf Time Zone*/
    {"H", 8 * 60},   /*Hotel: Hotel Time Zone*/
    {"I", 9 * 60},   /*India: Indian time zone*/
                     /*"J", Juliet is not used as a time zone, it is used to indicate the observer's local time.*/
    {"K", 10 * 60},  /*Kilo: Kilo Time Zone*/
    {"L", 11 * 60},  /*Lima: Lima Time Zone*/
    {"M", 12 * 60},  /*Mike: Mike Time Zone*/
    {"N", -1 * 60},  /*November: November time zone*/
    {"O", -2 * 60},  /*Oscar: Oscar Time Zone*/
    {"P", -3 * 60},  /*Papa: Papa Time Zone*/
    {"Q", -4 * 60},  /*Quebec: Quebec time zone*/
    {"R", -5 * 60},  /*Romeo: Romeo Time Zone*/
    {"S", -6 * 60},  /*Sierra: Sierra Time Zone*/
    {"T", -7 * 60},  /*Tango: Tango Time Zone*/
    {"U", -8 * 60},  /*Uniform: Uniform Time Zone*/
    {"V", -9 * 60},  /*Victor: Victoria Time Zone*/
    {"W", -10 * 60}, /*Whiskey: Whiskey Time Zone*/
    {"X", -11 * 60}, /*X-ray: X-ray time zone*/
    {"Y", -12 * 60}, /*Yankee: Yankee Time Zone*/
    {"Z", 0},        /*Zulu, zero meridian, a.k.a. UTC: Coordinated Universal Time*/
};

/*Internal Functions*/
/** Auxiliary Functions*/
static void pd_skip(const char **date)
{
    /*Skip all symbols that are not letters or numbers*/
    while (**date && !qurl_ctype_isalnum(**date))
    {
        (*date)++;
    }
}

/**
 * @brief Return the day of the week
 *
 * @param check
 * @param len
 * @return int -1: No day of the week. 0-6 represent Monday to Sunday respectively.
 */
static int pd_checkday(const char *check, long len)
{
    int                i = 0;
    const char *const *what;

    if (len > 3)
    {
        what = &qurl_pd_weekday[0];
    }
    else if (len == 3)
    {
        what = &qurl_pd_wkday[0];
    }
    else
    {
        return -1; /* too short */
    }
    for (i = 0; i < 7; i++)
    {
        long ilen = qurl_strlen(what[0]);
        if ((ilen == len) && qurl_strncasecmp(check, what[0], len))
        {
            return i;
        }
        what++;
    }
    return -1;
}

/**
 * @brief Return month index
 *
 * @param check
 * @param len
 * @return int
 */
static int pd_checkmonth(const char *check, long len)
{
    int                i = 0;
    const char *const *what = &qurl_pd_month[0];
    if (len != 3)
    {
        return -1; /*No month*/
    }

    for (i = 0; i < 12; i++)
    {
        if (qurl_strncasecmp(check, what[0], 3))
        {
            return i;
        }
        what++;
    }
    return -1; /*Returns the offset or -1, with no actual offset being -1.*/
}

/**
 * @brief Returns the time offset between the input time zone and GMT in seconds.
 * If no valid time zone is found, return -1
 *
 * @param check
 * @param len
 * @return int
 */
static int pd_checktz(const char *check, long len)
{
    unsigned int             i = 0;
    const struct pd_tz_info *what = tz;

    if (len > 4) /*Longer than any valid time zone*/
    {
        return -1;
    }

    for (i = 0; i < sizeof(tz) / sizeof(tz[0]); i++)
    {
        long ilen = qurl_strlen(what->name);
        if ((ilen == len) && qurl_strncasecmp(check, what->name, len))
        {
            return what->offset * 60;
        }
        what++;
    }
    return -1;
}

/**
 * @brief  Returns a single-digit or two-digit decimal value, and returns the pointer after the number.
 *
 * @param date
 * @param endp
 * @return int
 */
static int pd_oneortwodigit(const char *date, const char **endp)
{
    int num = date[0] - '0';
    if (qurl_ctype_isdigit(date[1]))
    {
        *endp = &date[2];
        return num * 10 + (date[1] - '0');
    }
    *endp = &date[1];

    return num;
}

/**
 * @brief HH:MM:SS or HH:MM also accepts single digits
 *
 * @param date
 * @param h
 * @param m
 * @param s
 * @param endp
 * @return true
 * @return false
 */
static qurl_bit_t pd_match_time(const char *date, int *h, int *m, int *s, char **endp)
{
    const char *p = QOSA_NULL;
    int         hh = 0;
    int         mm = 0;
    int         ss = 0;

    hh = pd_oneortwodigit(date, &p);
    if ((hh < 24) && (*p == ':') && qurl_ctype_isdigit(p[1]))
    {
        mm = pd_oneortwodigit(&p[1], &p);
        if (mm < 60)
        {
            if ((*p == ':') && qurl_ctype_isdigit(p[1]))
            {
                ss = pd_oneortwodigit(&p[1], &p);
                if (ss <= 60)
                {
                    /* valid HH:MM:SS */
                    goto match;
                }
            }
            else
            {
                /* valid HH:MM */
                goto match;
            }
        }
    }

    return QOSA_FALSE; /*No time string*/

match:
    *h = hh;
    *m = mm;
    *s = ss;
    *endp = (char *)p;
    return QOSA_TRUE;
}

/**
 * @brief Convert timestamp to seconds since the epoch in GMT timezone. Similar to mktime, but only for GMT.
 *
 * @param sec
 * @param min
 * @param hour
 * @param mday
 * @param mon
 * @param year
 * @return qurl_time_t
 */
static qurl_time_t pd_time2epoch(int sec, int min, int hour, int mday, int mon, int year)
{
    static const int month_days_cumulative[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int              leap_days = year - (mon <= 1);
    leap_days = ((leap_days / 4) - (leap_days / 100) + (leap_days / 400) - (1969 / 4) + (1969 / 100) - (1969 / 400));
    return ((((time_t)(year - 1970) * 365 + leap_days + month_days_cumulative[mon] + mday - 1) * 24 + hour) * 60 + min) * 60 + sec;
}

/** Functional Functions*/
static int pd_parsedate(const char *date, qurl_time_t *output)
{
    qurl_time_t      t = 0;
    int              wdaynum = -1; /* day of the week number, 0-6 (mon-sun) */
    int              monnum = -1;  /* month of the year number, 0-11 */
    int              mdaynum = -1; /* day of month, 1 - 31 */
    int              hournum = -1;
    int              minnum = -1;
    int              secnum = -1;
    int              yearnum = -1;
    int              tzoff = -1;
    qurl_pd_assume_e dignext = QURL_PD_ASSUNE_DATE_MDAY;
    const char      *indate = date; /* save the original pointer */
    int              part = 0;      /* max 6 parts */

    while (*date && (part < 6))
    {
        qurl_bit_t found = QOSA_FALSE;

        pd_skip(&date);

        if (qurl_ctype_isalpha(*date))
        {
            /* a name coming up */
            qosa_size_t len = 0;
            const char *p = date;
            while (qurl_ctype_isalpha(*p) && (len < PD_NAME_LEN))
            {
                p++;
                len++;
            }

            if (len != PD_NAME_LEN)
            {
                if (wdaynum == -1)
                {
                    wdaynum = pd_checkday(date, len);
                    if (wdaynum != -1)
                        found = QOSA_TRUE;
                }
                if (!found && (monnum == -1))
                {
                    monnum = pd_checkmonth(date, len);
                    if (monnum != -1)
                        found = QOSA_TRUE;
                }

                if (!found && (tzoff == -1))
                {
                    /*This must be a time zone string*/
                    tzoff = pd_checktz(date, len);
                    if (tzoff != -1)
                        found = QOSA_TRUE;
                }
            }
            if (!found)
                return QURL_PD_ECODE_FAIL; /* bad string */

            date += len;
        }
        else if (qurl_ctype_isdigit(*date))
        {
            /* a digit */
            int   val = 0;
            char *end = QOSA_NULL;
            if ((secnum == -1) && pd_match_time(date, &hournum, &minnum, &secnum, &end))
            {
                /* time stamp */
                date = end;
            }
            else
            {
                long lval = 0;

                lval = qurl_strtol(date, &end, 10);

#if LONG_MAX != INT_MAX
                if ((lval > (long)INT_MAX) || (lval < (long)INT_MIN))
                    return QURL_PD_ECODE_FAIL;
#endif

                val = (int)(lval);

                if ((tzoff == -1) && ((end - date) == 4) && (val <= 1400) && (indate < date) && ((date[-1] == '+' || date[-1] == '-')))
                {
                    /*Four digits, a value less than or equal to 1400 (to account for various interesting time zone differences) and prefixed with a plus or minus sign. This is a time zone indicator.
1400 was chosen because +1300 is frequently used, and the "ISO C 200X Proposal: Timezone Functions" document mentions +1400 as a boundary value.*/
                    found = QOSA_TRUE;
                    tzoff = (val / 100 * 60 + val % 100) * 60;

                    /*When using plus/minus signs as prefixes to indicate the offset of local time relative to GMT, reverse calculation is required.
For example, if a timestamp is represented as "+0300", it means the local time is 3 hours ahead of GMT. After performing the reverse calculation, we can determine that GMT time is 3 hours behind the local time.*/
                    tzoff = date[-1] == '+' ? -tzoff : tzoff;
                }

                if (((end - date) == 8) && (yearnum == -1) && (monnum == -1) && (mdaynum == -1))
                {
                    /*An 8-digit number representing a date, but without specifying the exact year, month, and day. The format is YYYYMMDD.*/
                    found = QOSA_TRUE;
                    yearnum = val / 10000;
                    monnum = (val % 10000) / 100 - 1; /*Month 0-11*/
                    mdaynum = val % 100;
                }

                if (!found && (dignext == QURL_PD_ASSUNE_DATE_MDAY) && (mdaynum == -1))
                {
                    if ((val > 0) && (val < 32))
                    {
                        mdaynum = val;
                        found = QOSA_TRUE;
                    }
                    dignext = QURL_PD_ASSUNE_DATE_YEAR;
                }

                if (!found && (dignext == QURL_PD_ASSUNE_DATE_YEAR) && (yearnum == -1))
                {
                    yearnum = val;
                    found = QOSA_TRUE;
                    if (yearnum < 100)
                    {
                        if (yearnum > 70)
                            yearnum += 1900;
                        else
                            yearnum += 2000;
                    }
                    if (mdaynum == -1)
                        dignext = QURL_PD_ASSUNE_DATE_MDAY;
                }

                if (!found)
                    return QURL_PD_ECODE_FAIL;

                date = end;
            }
        }

        part++;
    }

    if (-1 == secnum)
        secnum = minnum = hournum = 0; /* no time, make it zero */

    if ((-1 == mdaynum) || (-1 == monnum) || (-1 == yearnum))
        /*Missing critical information, parsing failed*/
        return QURL_PD_ECODE_FAIL;

    if (sizeof(qurl_time_t) < 5)
    {
        /*A signed 32-bit qurl_time_t can only store dates up to early 2038.*/
        if (yearnum > 2037)
        {
            *output = QURL_TIME_T_MAX;
            return QURL_PD_ECODE_LATER;
        }
        if (yearnum < 1903)
        {
            *output = QURL_TIME_T_MIN;
            return QURL_PD_ECODE_SOONER;
        }
    }
    else
    {
        /*The Gregorian calendar was introduced in 1582.*/
        if (yearnum < 1583)
        {
            return QURL_PD_ECODE_FAIL;
        }
    }

    if ((mdaynum > 31) || (monnum > 11) || (hournum > 23) || (minnum > 59) || (secnum > 60))
    {
        return QURL_PD_ECODE_FAIL; /*Obviously illegal*/
    }

    t = pd_time2epoch(secnum, minnum, hournum, mdaynum, monnum, yearnum);

    /*Add the time zone difference between the local time zone and GMT.*/
    if (tzoff == -1)
    {
        tzoff = 0;
    }

    if ((tzoff > 0) && (t > QURL_TIME_T_MAX - tzoff))
    {
        *output = QURL_TIME_T_MAX;
        return QURL_PD_ECODE_LATER; /* qurl_time_t overflow */
    }

    t += tzoff;

    *output = t;

    return QURL_PD_ECODE_OK;
}

/* API */
qurl_time_t qurl_pd_getdate(const char *p)
{
    qurl_time_t parsed = -1;
    int         rc = pd_parsedate(p, &parsed);

    if (rc == QURL_PD_ECODE_OK)
    {
        if (parsed == -1)
        {
            /*Avoid returning -1*/
            parsed++;
        }
        return parsed;
    }
    /*All others are fail*/
    return -1;
}

qurl_time_t qurl_pd_getdate_capped(const char *p)
{
    qurl_time_t parsed = -1;
    int         rc = pd_parsedate(p, &parsed);

    switch (rc)
    {
        case QURL_PD_ECODE_OK:
            if (parsed == -1)
            {
                /*Avoid returning -1*/
                parsed++;
            }
            return parsed;
        case QURL_PD_ECODE_LATER:
            /*Overflow, returns the maximum time value*/
            return parsed;
        default:
            return -1;
    }
}

#endif