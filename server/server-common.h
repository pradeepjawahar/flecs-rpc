#ifndef __SERVER_COMMON_H__
#define __SERVER_COMMON_H__

#include <log4cxx/logger.h>

#include "flecs.h"


namespace FleCS
{
	namespace ServerImpl
	{
		void AddServers(
				const std::vector<std::string>& servers,
				const Ice::CommunicatorPtr& comm);

		void AddServer(
				const std::string& endpoint,
				const Ice::CommunicatorPtr& comm);

		extern const char* stg_root_dir;
		extern std::map<std::string, FleCS::ServerPrx*> _servers;
	};
};


extern log4cxx::LoggerPtr logger;
#define _LOG(A) LOG4CXX_INFO(logger, A)


#endif
