#include <iterator>
#include <stdexcept>

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <log4cxx/logger.h>

#include "masterI.h"

#define _LOG(A) LOG4CXX_INFO(logger, A)

using namespace std;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("master"));


static FleCS::SM2SPrx* _new_proxy(
		const Ice::Current& cur,
		const string& endpoint)
{
	Ice::CommunicatorPtr ic = cur.adapter->getCommunicator();

	FleCS::SM2SPrx* s_prx = new FleCS::SM2SPrx;

	*s_prx = FleCS::SM2SPrx::checkedCast(
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


void MasterI::Join(
		const string& endpoint_,
		std::vector<std::string>& existingServers,
		const Ice::Current& cur)
{
	IceUtil::Mutex::Lock lock(_lock);

	_LOG("endpoint: " << endpoint_);
	// endpoint example: "Server:tcp -h meego -p 10000"

	Ice::ConnectionInfoPtr info = cur.con->getInfo();
	Ice::TCPConnectionInfoPtr tcpInfo = Ice::TCPConnectionInfoPtr::dynamicCast(info);

	if (!tcpInfo)
	{
		_LOG("Can not get tcpInfo.");
		exit(EXIT_FAILURE);
	}

	string endpoint = string("server: ") + endpoint_ + " -h " + tcpInfo->remoteAddress;

	// check if the endpoint already exists.
	map<string, FleCS::SM2SPrx*>::iterator i = _servers.find(endpoint);

	if (i != _servers.end())
	{
		_LOG("A server with endpoint " << endpoint << " already exist! Replacing with a new one.");

		// remove old proxy.
		delete i->second;
	}

	_servers[endpoint] = _new_proxy(cur, endpoint);

	// notify all other servers.
	for (map<string, FleCS::SM2SPrx*>::const_iterator i = _servers.begin(); i != _servers.end(); ++ i)
	{
		_LOG("  " << i->first);

		if (i->first == endpoint)
			continue;
		
		existingServers.push_back(i->first);

		(*(i->second))->ServerJoined(endpoint);
	}
}


std::vector<std::string> MasterI::GetLockServers(
		const Ice::Current&)
{
	IceUtil::Mutex::Lock lock(_lock);

	static bool first_request = true;
	static vector<string> servers;

	if (first_request)
	{
		first_request = false;

		for (map<string, FleCS::SM2SPrx*>::const_iterator i = _servers.begin(); i != _servers.end(); ++ i)
			servers.push_back(i->first);
	}
	else
	{
		// check the list of servers is not changed.
		
		vector<string> servers2;

		for (map<string, FleCS::SM2SPrx*>::const_iterator i = _servers.begin(); i != _servers.end(); ++ i)
			servers2.push_back(i->first);

		if (servers != servers2)
		{
			ostringstream err("server list has changed after the first request. servers: ");

			// string err = "server list has changed after the first request. servers: ";

			copy(servers.begin(), servers.end(), ostream_iterator<string>(err, " "));

			err << "servers2: ";
			copy(servers2.begin(), servers2.end(), ostream_iterator<string>(err, " "));

			throw runtime_error(err.str());
		}
	}

	return servers;
}
