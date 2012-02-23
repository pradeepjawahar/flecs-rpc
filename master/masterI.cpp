#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <log4cxx/logger.h>

#include "masterI.h"

#define _LOG(A) LOG4CXX_INFO(logger, (A))

using namespace std;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("master"));


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
		_LOG("invalid proxy");
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

	_LOG(string("endpoint: ") + endpoint_);

	Ice::ConnectionInfoPtr info = cur.con->getInfo();
	Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);

	if (!tcpInfo)
	{
		_LOG("Can not get tcpInfo.");
		exit(EXIT_FAILURE);
	}

	string endpoint = string("server: ") + endpoint_ + " -h " + tcpInfo->remoteAddress;

	// check if the endpoint already exists.
	map<string, FleCS::ServerPrx*>::iterator i = _servers.find(endpoint);

	if (i != _servers.end())
	{
		_LOG(string("A server with endpoint ") + endpoint
				+ " already exist! Replacing with a new one.");

		// remove old proxy.
		delete i->second;
	}

	_servers[endpoint] = _new_proxy(cur, endpoint);

	// notify all other servers.
	for (map<string, FleCS::ServerPrx*>::const_iterator i = _servers.begin(); i != _servers.end(); ++ i)
	{
		_LOG(string("  ") + i->first);

		if (i->first == endpoint)
			continue;
		
		existingServers.push_back(i->first);

		(*(i->second))->ServerJoined(endpoint);
	}
}
