#ifndef __AGENT_COMMON_H__
#define __AGENT_COMMON_H__

#include <log4cxx/logger.h>

extern log4cxx::LoggerPtr logger;
#define _LOG(A) LOG4CXX_INFO(logger, (A))

#endif
