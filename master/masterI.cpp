#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>

#include "masterI.h"

using namespace std;

#define cout_debug cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << "\n"


FleCS::Server1Prx* _new_proxy(
		const Ice::Current& cur,
		const string& endpoint)
{
	Ice::CommunicatorPtr ic = cur.adapter->getCommunicator();

	FleCS::Server1Prx* s_prx = new FleCS::Server1Prx;

	*s_prx = FleCS::Server1Prx::checkedCast(
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


// endpoint example: "Server:tcp -h meego -p 10000"
void MasterI::ServerReady(
		const string& endpoint_,
		const Ice::Current& cur)
{
	cout << __PRETTY_FUNCTION__ << "(" << endpoint_ << ")\n";

	Ice::ConnectionInfoPtr info = cur.con->getInfo();
	Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);

	if (!tcpInfo)
	{
		cerr << __FUNCTION__ << "Can not get tcpInfo.\n";
		exit(EXIT_FAILURE);
	}

	string endpoint = string("server1: ") + endpoint_ + " -h " + tcpInfo->remoteAddress;

	// check if the endpoint already exists.
	map<string, FleCS::Server1Prx*>::iterator i = _servers.find(endpoint);

	if (i != _servers.end())
	{
		cout << "A server with endpoint " << endpoint << " already exist!\n";
		cout << "Replacing with a new one.\n";

		// remove old proxy.
		delete i->second;
	}

	_servers[endpoint] = _new_proxy(cur, endpoint);

	// notify all other servers.
	cout << "  ";
	for (map<string, FleCS::Server1Prx*>::const_iterator i = _servers.begin(); i != _servers.end(); ++ i)
	{
		cout << i->first << " ";

		if (i->first == endpoint)
			continue;
		
		(*(i->second))->ServerAdded(endpoint);
	}

	cout << "\n";
}
