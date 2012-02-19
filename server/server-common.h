#ifndef __SERVER_COMMON_H__
#define __SERVER_COMMON_H__

#include "flecs.h"


namespace FleCS
{
	namespace ServerImpl
	{
		void AddServer(
				const std::string& endpoint,
				const Ice::Current& cur);

		extern const char* stg_root_dir;

		extern std::map<std::string, FleCS::Server1Prx*> _servers;
	};
};

#endif
