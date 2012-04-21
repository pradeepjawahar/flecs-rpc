#include <boost/program_options.hpp>
#include <Ice/Ice.h>

#include "server-common.h"

using namespace std;


boost::program_options::variables_map povm;


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


FleCS::MasterPrx& FleCS::ServerImpl::GetMasterProxy()
{
	static FleCS::MasterPrx prx = FleCS::MasterPrx::checkedCast(
			Ice::Application::communicator()
			->stringToProxy(string("master:tcp -p 10001 -h ") + povm["master"].as<string>())
			->ice_twoway()
			->ice_timeout(-1)
			->ice_secure(false));
	if(!prx)
		throw runtime_error("invalid proxy");

	return prx;
}


const char* FleCS::ServerImpl::stg_root_dir = NULL;


// TODO: Delete proxies to other servers. After app.main(). Need to test if
// they can be safely released.
std::map<std::string, FleCS::ServerPrx*> FleCS::ServerImpl::_servers;


FleCS::ServerImpl::GlobalLock::GlobalLock(
		const std::string& path,
		const char type)
{
	if (_initialized == false)
		_Init();

	// TODO: acquire
	//IceUtil::Mutex::Lock lock(_lock);
}


FleCS::ServerImpl::GlobalLock::~GlobalLock()
{
	// TODO: release
}


void FleCS::ServerImpl::GlobalLock::_Init()
{
	// get a list of lock servers from the master.  master needs a
	// lock for the operation. and after serving the first request,
	// the lock server list will remain unchanged.
	FleCS::MasterPrx& mp = GetMasterProxy();
	std::vector<std::string> lock_servers = mp->GetLockServers();

	ostringstream oss;
	copy(lock_servers.begin(), lock_servers.end(), ostream_iterator<string>(oss, " "));
	_LOG(oss.str());

	// A lock is not needed here. Running this function multiple
	// times is harmless.

	_initialized = true;
}


bool FleCS::ServerImpl::GlobalLock::_initialized = false;


log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("server"));
