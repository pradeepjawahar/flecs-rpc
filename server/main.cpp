#include <Ice/Ice.h>

#include "server-common.h"
#include "util.h"
#include "c2sI.h"
#include "serverI.h"

using namespace std;


class FleCSServer : public Ice::Application
{
public:
	FleCSServer()
	{
		// create server root dir
		_create_directories(FleCS::ServerImpl::stg_root_dir);
	}
	

    virtual int run(int, char*[])
	{
		shutdownOnInterrupt();

		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("FleCS");

		FleCS::C2SPtr c2s = new C2SI;
		adapter->add(c2s, communicator()->stringToIdentity("c2s"));

		FleCS::ServerPtr s2s = new ServerI;
		adapter->add(s2s, communicator()->stringToIdentity("server"));

		adapter->activate();


		// Notify master that a server is on.
		//
		// No deadlock as long as the master do not notify the originating server.
		//   server --(join)--> master --(notify)--> all other servers.
		//
		// Master needs to make sure that join service is serialized.
		//
		// If server empolyes multiple thread with a separate dispatch thread,
		// there won't be a deadlock in any case.
		
		FleCS::MasterPrx m_prx = FleCS::MasterPrx::checkedCast(
				communicator()
				->propertyToProxy("FleCS_master.Proxy")
				->ice_twoway()
				->ice_timeout(-1)
				->ice_secure(false));
		if(!m_prx)
		{
			_LOG("invalid proxy m_prx");
			exit(EXIT_FAILURE);
		}

		// Ask to join the system. Give my endpoint and get the existing
		// servers.

		Ice::EndpointSeq eps = adapter->getEndpoints();
		// Assume that this server has one endpoint.
		if (eps.size() != 1)
		{
			_LOG("Unexpected");
			exit(EXIT_FAILURE);
		}

		vector<string> existingServers;
		m_prx->Join((*eps.begin())->toString(), existingServers);
		FleCS::ServerImpl::AddServers(existingServers, communicator());

		communicator()->waitForShutdown();

		return EXIT_SUCCESS;
	}
};


int main(int argc, char* argv[])
{
	int rc;

	cout << "server started.\n";

	if (daemon(1, 0) == -1)
	{
		perror("daemon");
		return EXIT_FAILURE;
	}

	_LOG("server starting...");

	try
	{
		FleCSServer app;
		rc = app.main(argc, argv, "config.server");
	}
	catch(const exception& e)
	{
		_LOG(e.what());
	}

	_LOG("server finished.");

	return rc;
}
