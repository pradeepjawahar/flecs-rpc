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
			cerr << "invalid proxy m_prx" << endl;
			exit(EXIT_FAILURE);
		}

		// Assume that this server has one endpoint.
		Ice::EndpointSeq eps = adapter->getEndpoints();
		if (eps.size() != 1)
		{
			cerr << "Unexpected" << endl;
			exit(EXIT_FAILURE);
		}

		// Join the system.
		vector<string> existingServers;
		m_prx->Join((*eps.begin())->toString(), existingServers);
		FleCS::ServerImpl::AddServers(existingServers, communicator());

		communicator()->waitForShutdown();

		cout << "Serverr closing.\n";
		return EXIT_SUCCESS;
	}
};


int main(int argc, char* argv[])
{
    FleCSServer app;
    return app.main(argc, argv, "config.server");
}
