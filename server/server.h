#include <boost/program_options.hpp>
#include <log4cxx/logger.h>
#include <Ice/Ice.h>

#include "flecs.h"


extern boost::program_options::variables_map povm;

extern log4cxx::LoggerPtr logger;
#define _LOG(A) LOG4CXX_INFO(logger, A)


FleCS::SM2SPrx* _new_sm2s_proxy(const std::string& endpoint);


class FleCSServer : public Ice::Application
{
public:
	FleCSServer();
	~FleCSServer();
	
    virtual int run(int, char*[]);

	static void AddServer(const std::string& endpoint);

	static FleCS::MasterPrx& GetMasterProxy();

	static const char* stg_root_dir;

	static std::map<std::string, FleCS::SM2SPrx*> peer_servers;

private:
	void AddServers(const std::vector<std::string>& servers);
};


void parse_args(int argc, char* argv[]);
