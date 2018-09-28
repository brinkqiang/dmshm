
#ifndef __GSHMAGENT_H_INCLUDE__
#define __GSHMAGENT_H_INCLUDE__
#include "dmos.h"
#include "dmtypes.h"
#include "dmsingleton.h"
#include "dmformat.h"
#include "dmshm.h"

typedef enum dmlog_levels_e {
    DMLOG_LEVEL_OFF = -1,
    DMLOG_LEVEL_FATAL = 0,
    DMLOG_LEVEL_ERROR = 1,
    DMLOG_LEVEL_WARN = 2,
    DMLOG_LEVEL_INFO = 3,
    DMLOG_LEVEL_DEBUG = 4,
    DMLOG_LEVEL_TRACE = 5,
    DMLOG_LEVEL_ALL = DMLOG_LEVEL_TRACE
} DMLogLevels;

typedef enum dmlog_msg_levels_e {
    DMLOG_MSG_LEVEL_FATAL = DMLOG_LEVEL_FATAL,
    DMLOG_MSG_LEVEL_ERROR = DMLOG_LEVEL_ERROR,
    DMLOG_MSG_LEVEL_WARN = DMLOG_LEVEL_WARN,
    DMLOG_MSG_LEVEL_INFO = DMLOG_LEVEL_INFO,
    DMLOG_MSG_LEVEL_DEBUG = DMLOG_LEVEL_DEBUG,
    DMLOG_MSG_LEVEL_TRACE = DMLOG_LEVEL_TRACE
} DMLogMsgLevels;

class CDMShmAgent : 
    public CDMSafeSingleton<CDMShmAgent>
{
    friend class CDMSafeSingleton<CDMShmAgent>;
public:
    CDMShmAgent();

    bool Init();
    void Log(DMLogMsgLevels level, const char *srcFilename, int srcLine, const std::string &message);
private:
    bool __LoadCSV();
private:
    DMShmem m_oShmem;
    DMShmConfig m_oConfig;
    int m_nIndex;
};

#define DMSHM_AGENT_INIT() if(!CDMShmAgent::Instance()->Init()){ exit(-1);}

#define LOG_FATAL(...) CDMShmAgent::Instance()->Log(DMLOG_MSG_LEVEL_FATAL, __FILE__, __LINE__, std::move(fmt::format(__VA_ARGS__)))
#define LOG_ERROR(...) CDMShmAgent::Instance()->Log(DMLOG_MSG_LEVEL_ERROR, __FILE__, __LINE__, std::move(fmt::format(__VA_ARGS__)))
#define LOG_WARN(...)  CDMShmAgent::Instance()->Log(DMLOG_MSG_LEVEL_WARN,  __FILE__, __LINE__, std::move(fmt::format(__VA_ARGS__)))
#define LOG_INFO(...)  CDMShmAgent::Instance()->Log(DMLOG_MSG_LEVEL_INFO,  __FILE__, __LINE__, std::move(fmt::format(__VA_ARGS__)))
#define LOG_DEBUG(...) CDMShmAgent::Instance()->Log(DMLOG_MSG_LEVEL_DEBUG, __FILE__, __LINE__, std::move(fmt::format(__VA_ARGS__)))
#define LOG_TRACE(...) CDMShmAgent::Instance()->Log(DMLOG_MSG_LEVEL_TRACE, __FILE__, __LINE__, std::move(fmt::format(__VA_ARGS__)))

#endif // __GSHMAGENT_H_INCLUDE__