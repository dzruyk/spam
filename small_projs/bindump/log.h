#ifndef _LOG_H_
#define _LOG_H_

#define IS_DEBUG 1
#define LOG_LEVEL LOG_DEFAULT

#define LOG_DEFAULT 1
#define LOG_VERBOSE 2

//WIP debug macro
#if IS_DEBUG == 1

#  define DEBUG(MSG_LOG_LVL, fmt, arg...) \
do {\
    if (MSG_LOG_LVL <= LOG_LEVEL) {\
        fprintf(stderr, "%s:"fmt,\
        __FUNCTION__, \
        ##arg); \
    } \
} while (0)

#else
#  define DEBUG(LOG_LVL, fmt, arg...)
#endif

#endif
