//
// C++ Interface: k9common
//
// Description: 
//
// Developer: Pauline123 <pauline123@users.sf.net>, (C) 2014
//
// Author: Jean-Michel PETIT <k9copy@free.fr>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "config.h"
#ifndef __STDC_LIMIT_MACROS
#endif
#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif

#if defined(HAVE_INTTYPES_H)
    #include <inttypes.h>
#elif defined( HAVE_STDINT_H)
     #include <stdint.h>
#endif

#if defined(HAVE_TIME_H)
//TODO:PTZ161103 screewy-dooby __USE_XOPEN2K8 features
#include <time.h>
#if ! defined(__timespec_defined)
# define __timespec_defined	1
# include <bits/types.h>	/* This defines __time_t for us.  */
struct timespec
{
    __time_t tv_sec;		/* Seconds.  */
    __syscall_slong_t tv_nsec;	/* Nanoseconds.  */
};
#endif
#endif

#ifdef __FreeBSD__
    #include <osreldate.h>
#endif
#include "dvdread/ifo_types.h" //PM DVDREAD
#include "dvdread/dvd_reader.h" //PM DVDREAD
#include "dvdread/ifo_read.h" //PM DVDREAD
#include "dvdread/nav_read.h" //PM DVDREAD
#include "dvdread/nav_types.h" //PM DVDREAD

#include <QObject>

#ifndef DVD_BLOCK_LEN
#define DVD_BLOCK_LEN 2048
#endif

#include "k9dvdsize.h"

#include <QtDebug>
#include "k9dialogs.h"

#include "k9log.h"
#include "k9tools.h"



#include <KDE/KLocale>
///usr/include/KDE/KLocalizedDate
#include <KDE/KLocalizedString>
#include <KF5/KI18n/KLocalizedString>
#if QT_VERSION >= 0x050000
//#include <KLocalizedString>
#else //Qt4 Code
#include <KLocale>
#endif


#include "k9config.h"
#include "k9process.h"



