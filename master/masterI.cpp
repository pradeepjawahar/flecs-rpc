#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>

#include "masterI.h"

using namespace std;

#define cout_debug cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << "\n"


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


// endpoint example: "Server:tcp -h meego -p 10000"
void MasterI::Join(
		const string& endpoint_,
		std::vector<std::string>& existingServers,
		const Ice::Current& cur)
{
	IceUtil::Mutex::Lock lock(_lock);

	cout << "MasterI::" << __FUNCTION__ << "(" << endpoint_ << ")\n";

	Ice::ConnectionInfoPtr info = cur.con->getInfo();
	Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);

	if (!tcpInfo)
	{
		cerr << __FUNCTION__ << "Can not get tcpInfo.\n";
		exit(EXIT_FAILURE);
	}

	string endpoint = string("server: ") + endpoint_ + " -h " + tcpInfo->remoteAddress;

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

	// notify all other servers.
	for (map<string, FleCS::ServerPrx*>::const_iterator i = _servers.begin(); i != _servers.end(); ++ i)
	{
		cout << "  " << i->first << "\n";

		if (i->first == endpoint)
			continue;
		
		existingServers.push_back(i->first);

		(*(i->second))->ServerJoined(endpoint);
	}
}
