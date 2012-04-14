#include <Ice/Ice.h>

#include "server-common.h"

using namespace std;


static FleCS::ServerPrx* _new_proxy(
		const string& endpoint,
		const Ice::CommunicatorPtr& comm)
{
	FleCS::ServerPrx* s_prx = new FleCS::ServerPrx;

	*s_prx = FleCS::ServerPrx::checkedCast(
			comm
			->stringToProxy(endpoint)
			->ice_twoway()
			->ice_timeout(-1)
			->ice_secure(false));
	if(!s_prx)
	{
		_LOG("invalid proxy");
		exit(EXIT_FAILURE);
	}

	return s_prx;
}


void FleCS::ServerImpl::AddServers(
		const vector<string>& servers,
		const Ice::CommunicatorPtr& comm)
{
	for (vector<string>::const_iterator i = servers.begin(); i != servers.end(); ++ i)
	{
		if (i == servers.begin())
			_LOG("AddServers:");

		AddServer(*i, comm);
		_LOG("  " <<  *i);
	}
}


void FleCS::ServerImpl::AddServer(
		const string& endpoint,
		const Ice::CommunicatorPtr& comm)
{
	// check if the endpoint already exists.
	map<string, FleCS::ServerPrx*>::iterator i = _servers.find(endpoint);

	if (i != _servers.end())
	{
		_LOG("A server with endpoint " << endpoint << " already exist! Replacing with a new one.");

		// remove old proxy.
		delete i->second;
	}

	_servers[endpoint] = _new_proxy(endpoint, comm);
}


const char* FleCS::ServerImpl::stg_root_dir = NULL;


// TODO: Delete proxies to other servers. After app.main(). Need to test if
// they can be safely released.
std::map<std::string, FleCS::ServerPrx*> FleCS::ServerImpl::_servers;


log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("server"));
