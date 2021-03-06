#ifndef __MYLOG_H__
#define __MYLOG_H__

#define IS_DEBUG 1
#define LOG_LEVEL LOG_VERBOSE

#define LOG_DEFAULT 1
#define LOG_VERBOSE 2

//WIP debug macro
#if IS_DEBUG == 1

#define DEBUG(MSG_LOG_LVL, fmt, arg...) \
do {\
    if (MSG_LOG_LVL <= LOG_LEVEL) {\
        fprintf(stderr, "function %s:"fmt,\
        __FUNCTION__, \
        ##arg); \
    } \
} while (0)

#endif

#endif
