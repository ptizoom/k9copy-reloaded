//
// C++ Interface: k9time
//
// Description: header paliating problems with <chrono>
//
//
// Author: PTZ <ptizoom@gmail.com>, (C) 2019
//
// Copyright: See COPYING file that comes with this distribution
//
//
#pragma once
#include "config.h"
#if defined(HAVE_TIME_H)
//TODO:PTZ190809  time shit with /usr/include/c++/8/chrono:41
// /usr/include/c++/8/ctime:60:11: error: ‘::clock_t’ has not been declared  using ::clock_t;
// etc...
#include <sys/types.h> //PTZ190810 needed  for clock_t, time_t
#include <time.h>      /* clock_t, clock, CLOCKS_PER_SEC */
extern "C" {
    clock_t clock (void);
    time_t mktime (struct tm * timeptr);
    double difftime (time_t end, time_t beginning);
    struct tm * gmtime (const time_t * timer);
    char* asctime (const struct tm * timeptr);
    time_t time (time_t* timer);
    char* ctime (const time_t * timer);
    struct tm * localtime (const time_t * timer);
    size_t strftime (char* ptr, size_t maxsize, const char* format,
                     const struct tm* timeptr );
    int nanosleep(const struct timespec *req, struct timespec *rem);
}
#endif

//TODO:PTZ161103 screewy-dooby __USE_XOPEN2K8 features
#if 0 && ! defined(__timespec_defined)
# define __timespec_defined	1
# include <bits/types.h>	/* This defines __time_t for us.  */
struct timespec
{
    __time_t tv_sec;		/* Seconds.  */
    __syscall_slong_t tv_nsec;	/* Nanoseconds.  */
};
#endif
