#include <Ice/Ice.h>

#include "server-common.h"
#include "util.h"

#include "c2sI.h"
#include "s2sI.h"

using namespace std;


class FleCSServer : public Ice::Application
{
public:
	FleCSServer()
	{
		// create server root dir
		_create_directories(FleCS::Server::stg_root_dir);
	}
	

    virtual int run(int, char*[])
	{
		shutdownOnInterrupt();

		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("FleCS");

		FleCS::C2SPtr c2s = new Imp_c2s;
		adapter->add(c2s, communicator()->stringToIdentity("c2s"));

		FleCS::Server1Ptr s2s = new ServerI;
		adapter->add(s2s, communicator()->stringToIdentity("server1"));

		adapter->activate();


		// Notify master that a server is on.

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

		m_prx->ServerReady((*eps.begin())->toString());

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
