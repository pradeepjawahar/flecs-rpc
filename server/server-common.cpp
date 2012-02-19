#include <Ice/Ice.h>

#include "server-common.h"

using namespace std;


static FleCS::ServerPrx* _new_proxy(
		const Ice::Current& cur,
		const string& endpoint)
{
	Ice::CommunicatorPtr ic = cur.adapter->getCommunicator();

	FleCS::ServerPrx* s_prx = new FleCS::ServerPrx;

	*s_prx = FleCS::ServerPrx::checkedCast(
			ic
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


void FleCS::ServerImpl::AddServer(
		const string& endpoint,
		const Ice::Current& cur)
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

	_servers[endpoint] = _new_proxy(cur, endpoint);
}


const char* FleCS::ServerImpl::stg_root_dir = "/usr/local/flecs/no-cnst";


// TODO: When to delete a proxy? Unless server serializes access to the proxy
// lists or keep track of reference counter, it won't be safe to delete a
// proxy.
//
// Serialization is bad for scalability. Reference counting seems a little
// better. Not implemented yet.
std::map<std::string, FleCS::ServerPrx*> FleCS::ServerImpl::_servers;
