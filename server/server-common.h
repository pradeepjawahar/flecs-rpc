#ifndef __SERVER_COMMON_H__
#define __SERVER_COMMON_H__

#include <boost/program_options.hpp>

#include <log4cxx/logger.h>

#include "flecs.h"


extern boost::program_options::variables_map povm;


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

		FleCS::MasterPrx& GetMasterProxy();

		extern const char* stg_root_dir;
		extern std::map<std::string, FleCS::ServerPrx*> _servers;


		class GlobalLock
		{
		public:
			GlobalLock(
					const std::string& path,
					const char type);

			~GlobalLock();

		private:
			static void _Init();

			IceUtil::Mutex _locks_lock;

			std::map<std::string, IceUtil::Mutex*> _locks;

			std::vector<std::string> _lock_servers;

			static bool _initialized;
		};
	};
};


extern log4cxx::LoggerPtr logger;
#define _LOG(A) LOG4CXX_INFO(logger, A)


#endif
