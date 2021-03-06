#ifndef __STDX_LOG_H
#define __STDX_LOG_H

// C 89 header files
#include <errno.h>
#include <strings.h>

// C++ 98 header files
#include <list>
#include <string>
#include <sstream>
#include <iostream>

// Rain header files
#include "stdx_ttycolor.h"

#ifndef BRIDGE_ENTRY_SERVER
    #include "nb_configuration.h"
#else
    #ifdef HOST_KEEPER_PROCESS
        #include "hk_configuration.h"
    #else
        #include "be_configuration.h"
    #endif
#endif
//static unsigned logInfoNum;

namespace stdx {


#ifndef BRIDGE_ENTRY_SERVER
    #define LOG_OUTPUT(osscolor, statement, logType) \
    do                                  \
    {                                   \
        if(logType >= nb_configuration::instance().get_log_level()) \
        {                                   \
            std::ostringstream oss;         \
            oss << osscolor;                \
            oss << statement;               \
            oss << stdx::endcolor;          \
            oss << std::endl;               \
            std::cerr << oss.str();         \
        }                                   \
    } while (false)

#else
    #ifdef HOST_KEEPER_PROCESS
        #define LOG_OUTPUT(osscolor, statement, logType)    \
        do                                                      \
        {                                   \
            if(logType >= hk_configuration::instance().get_log_level()) \
            {                                                           \
                std::ostringstream oss;                                 \
                oss << osscolor;                                        \
                oss << statement;                                       \
                oss << stdx::endcolor;                                  \
                oss << std::endl;                                       \
                std::cerr << oss.str();                                 \
            }                                                           \
        } while (false)
    #else
        #define LOG_OUTPUT(osscolor, statement, logType)    \
        do                                  \
        {                                                           \
            if(logType >= be_configuration::instance().get_log_level()) \
            {                                                           \
                std::ostringstream oss;                                 \
                oss << osscolor;                                        \
                oss << statement;                                       \
                oss << stdx::endcolor;                                  \
                oss << std::endl;                                       \
                std::cerr << oss.str();                                 \
            }                                                           \
        } while (false)
    #endif
#endif

#define LOG_DEBUG(statement)    LOG_OUTPUT(stdx::endcolor, statement, 1)
#define LOG_INFO(statement)     LOG_OUTPUT(stdx::endcolor, statement, 2)
#define LOG_NOTICE(statement)   LOG_OUTPUT(stdx::yellow, statement, 3)
#define LOG_WARNING(statement)  LOG_OUTPUT(stdx::yellow, statement, 4)
#define LOG_ERROR(statement)    LOG_OUTPUT(stdx::red, statement, 5)
#define LOG_CRIT(statement)     LOG_OUTPUT(stdx::red, statement, 6)
#define LOG_ALERT(statement)    LOG_OUTPUT(stdx::red, statement, 7)
#define LOG_EMERG(statement)    LOG_OUTPUT(stdx::red, statement, 8)


#define LOG_FORMAT(format, ...) \
    do                                                  \
    {                                                   \
        int n = snprintf(NULL, 0, format, __VA_ARGS__); \
        if (n > 0)                                      \
        {                                               \
            char buf[n + 1];                            \
            snprintf(buf, n + 1, format, __VA_ARGS__);  \
            std::cerr << buf << std::endl;              \
        }                                               \
    } while (false)


/*
#define LOG_FORMAT_OUTPUT(osscolor, format, args) \
    do                                                  \
    {                                                   \
        int n = snprintf(NULL, 0, format, args);        \
        if (n > 0)                                      \
        {                                               \
            char buf[n + 1];                            \
            snprintf(buf, n + 1, format, args);         \
            LOG_OUTPUT(osscolor, buf);                  \
        }                                               \
    } while (false)

#define LOG_FORMAT_DEBUG(format, ...)    do { LOG_FORMAT_OUTPUT(stdx::endcolor, format, __VA_ARGS__); } while(false)
#define LOG_FORMAT_INFO(format, ...)     LOG_FORMAT_OUTPUT(stdx::endcolor, format, __VA_ARGS__)
#define LOG_FORMAT_NOTICE(format, ...)   LOG_FORMAT_OUTPUT(stdx::yellow, format, __VA_ARGS__)
#define LOG_FORMAT_WARNING(format, ...)  LOG_FORMAT_OUTPUT(stdx::yellow, format, __VA_ARGS__)
#define LOG_FORMAT_ERROR(format, ...)    LOG_FORMAT_OUTPUT(stdx::red, format, __VA_ARGS__)
#define LOG_FORMAT_CRIT(format, ...)     LOG_FORMAT_OUTPUT(stdx::red, format, __VA_ARGS__)
#define LOG_FORMAT_ALERT(format, ...)    LOG_FORMAT_OUTPUT(stdx::red, format, __VA_ARGS__)
#define LOG_FORMAT_EMERG(format, ...)    LOG_FORMAT_OUTPUT(stdx::red, format, __VA_ARGS__)
*/


#define LOG_PERROR(statement) \
    do                                          \
    {                                           \
        std::string strerr = strerror(errno);   \
        LOG_ERROR(statement << ": " << strerr); \
    } while (false)

class call_stack
{
private:
    typedef std::list<std::string>  __stack_type;
    __stack_type m_calls;

public:
    void push(const std::string& strcall)
    {
        m_calls.push_back(strcall);
    }

    void pop()
    {
        m_calls.pop_back();
    }

    std::string names() const
    {
        std::string strcalls;
        for (__stack_type::const_iterator it = m_calls.begin();
                it != m_calls.end(); ++it)
        {
            strcalls += " --> " + *it + "()";
        }
        return strcalls;
    }
};

class call_trace
{
private:
    call_stack& m_calls;

public:
    call_trace(call_stack& calls, const std::string& name) : m_calls(calls)
    {
        m_calls.push(name);
        LOG_DEBUG("STACK " << std::string(10,'+') << m_calls.names());
    }

    ~call_trace()
    {
        LOG_DEBUG("STACK " << std::string(10,'-') << m_calls.names());
        m_calls.pop();
    }
};


} // namespace stdx


#endif // __STDX_LOG_H

// vim:set tabstop=4 shiftwidth=4 expandtab:
