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
		cerr << "invalid proxy" << endl;
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
			cout << "AddServers:\n";

		AddServer(*i, comm);
		cout << "  " << *i << "\n";
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
		cout << "A server with endpoint " << endpoint << " already exist!\n";
		cout << "Replacing with a new one.\n";

		// remove old proxy.
		delete i->second;
	}

	_servers[endpoint] = _new_proxy(endpoint, comm);
}


const char* FleCS::ServerImpl::stg_root_dir = "/usr/local/flecs/no-cnst";


// TODO: When to delete a proxy? Unless server serializes access to the proxy
// lists or keep track of reference counter, it won't be safe to delete a
// proxy.
//
// Serialization is bad for scalability. Reference counting seems a little
// better. Not implemented yet.
std::map<std::string, FleCS::ServerPrx*> FleCS::ServerImpl::_servers;
