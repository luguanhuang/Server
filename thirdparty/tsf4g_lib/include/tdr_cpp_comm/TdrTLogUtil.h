/* This file is generated by tdr. */
/* No manual modification is permitted. */

/* tdr version: 2.7.12 */

#ifndef TDR_TLOG_UTIL_H_
#define TDR_TLOG_UTIL_H_

/* when using macros defined in this file, you may need to: */
/* 1. include tlog header file, by: #include "tlog/tlog.h"  */
/* 2. link tsf4g library file, such as -ltsf4g for makefile */

// category:    type: 'TLOGCATEGORYINST*'
// priority:    type: 'int'
// id:          type: 'unsigned int'
// cls:         type: 'unsigned int'
// buffer:      type: 'char*'
// size:        type: 'size_t'
// object:      anything with method of 'visualize(char*, size_t)'
#define TDRCPP_LOG(category, priority, id, cls, object, buffer, size)\
    do {\
        if (NULL == (category) || NULL == (buffer) || (size) <= 0)\
        {\
            TSF4G_TDR_DEBUG_TRACE();\
            break;\
        }\
        \
        if (!tlog_category_is_priority_enabled((category), (priority))\
            || !tlog_category_can_write((category), (priority), (id), (cls)))\
        {\
            break;\
        }\
        \
        ((char*)(buffer))[0] = 0;\
        (object).visualize((buffer), (size));\
        ((char*)(buffer))[(size)-1] = 0;\
        \
        tlog_log((category), (priority), (id), (cls), "\n%s", (buffer));\
    } while(0)

// category:    type: 'TLOGCATEGORYINST*'
// priority:    type: 'int'
// id:          type: 'unsigned int'
// cls:         type: 'unsigned int'
// object:      anything with method of 'visualize(char*, size_t)'
// size:        a positive integer:
//              if 'size' is a literal constant, such as '1024', use stack space as buffer;
//              otherwise, use 'variable-sized array'(ONLY make sense for stadard C++ compilers);
#define TDRCPP_VSA_LOG(category, priority, id, cls, object, size)\
    do {\
        char buffer[(size)];\
        TDRCPP_LOG((category), (priority), (id), (cls), (object), buffer, (size));\
    } while(0)


#define TDRCPP_LOG_FATAL(category, id, cls, object, buffer, size)\
    TDRCPP_LOG((category), TLOG_PRIORITY_FATAL, (id), (cls), (object), (buffer), (size))

#define TDRCPP_LOG_ALERT(category, id, cls, object, buffer, size)\
    TDRCPP_LOG((category), TLOG_PRIORITY_ALERT, (id), (cls), (object), (buffer), (size))

#define TDRCPP_LOG_CRIT(category, id, cls, object, buffer, size)\
    TDRCPP_LOG((category), TLOG_PRIORITY_CRIT, (id), (cls), (object), (buffer), (size))

#define TDRCPP_LOG_ERROR(category, id, cls, object, buffer, size)\
    TDRCPP_LOG((category), TLOG_PRIORITY_ERROR, (id), (cls), (object), (buffer), (size))

#define TDRCPP_LOG_WARN(category, id, cls, object, buffer, size)\
    TDRCPP_LOG((category), TLOG_PRIORITY_WARN, (id), (cls), (object), (buffer), (size))

#define TDRCPP_LOG_NOTICE(category, id, cls, object, buffer, size)\
    TDRCPP_LOG((category), TLOG_PRIORITY_NOTICE, (id), (cls), (object), (buffer), (size))

#define TDRCPP_LOG_INFO(category, id, cls, object, buffer, size)\
    TDRCPP_LOG((category), TLOG_PRIORITY_INFO, (id), (cls), (object), (buffer), (size))

#define TDRCPP_LOG_DEBUG(category, id, cls, object, buffer, size)\
    TDRCPP_LOG((category), TLOG_PRIORITY_DEBUG, (id), (cls), (object), (buffer), (size))

#define TDRCPP_LOG_TRACE(category, id, cls, object, buffer, size)\
    TDRCPP_LOG((category), TLOG_PRIORITY_TRACE, (id), (cls), (object), (buffer), (size))


#define TDRCPP_VSA_LOG_FATAL(category, id, cls, object, size)\
    TDRCPP_VSA_LOG((category), TLOG_PRIORITY_FATAL, (id), (cls), (object), (size))

#define TDRCPP_VSA_LOG_ALERT(category, id, cls, object, size)\
    TDRCPP_VSA_LOG((category), TLOG_PRIORITY_ALERT, (id), (cls), (object), (size))

#define TDRCPP_VSA_LOG_CRIT(category, id, cls, object, size)\
    TDRCPP_VSA_LOG((category), TLOG_PRIORITY_CRIT, (id), (cls), (object), (size))

#define TDRCPP_VSA_LOG_ERROR(category, id, cls, object, size)\
    TDRCPP_VSA_LOG((category), TLOG_PRIORITY_ERROR, (id), (cls), (object), (size))

#define TDRCPP_VSA_LOG_WARN(category, id, cls, object, size)\
    TDRCPP_VSA_LOG((category), TLOG_PRIORITY_WARN, (id), (cls), (object), (size))

#define TDRCPP_VSA_LOG_NOTICE(category, id, cls, object, size)\
    TDRCPP_VSA_LOG((category), TLOG_PRIORITY_NOTICE, (id), (cls), (object), (size))

#define TDRCPP_VSA_LOG_INFO(category, id, cls, object, size)\
    TDRCPP_VSA_LOG((category), TLOG_PRIORITY_INFO, (id), (cls), (object), (size))

#define TDRCPP_VSA_LOG_DEBUG(category, id, cls, object, size)\
    TDRCPP_VSA_LOG((category), TLOG_PRIORITY_DEBUG, (id), (cls), (object), (size))

#define TDRCPP_VSA_LOG_TRACE(category, id, cls, object, size)\
    TDRCPP_VSA_LOG((category), TLOG_PRIORITY_TRACE, (id), (cls), (object), (size))

#endif