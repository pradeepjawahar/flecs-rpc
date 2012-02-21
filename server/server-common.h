#ifndef __SERVER_COMMON_H__
#define __SERVER_COMMON_H__

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

#endif
