#include <boost/program_options.hpp>
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
		if (povm["storage"].as<string>() == "disk")
			FleCS::ServerImpl::stg_root_dir = "/usr/local/flecs/data";
		else if (povm["storage"].as<string>() == "memory")
			FleCS::ServerImpl::stg_root_dir = "/dev/shm/flecs/data";
		else
			throw runtime_error(string("Unknown storage: ") + FleCS::ServerImpl::stg_root_dir);

		// create server root dir
		_create_directories(FleCS::ServerImpl::stg_root_dir);
	}
	

    virtual int run(int, char*[])
	{
		try
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

			FleCS::MasterPrx& m_prx = FleCS::ServerImpl::GetMasterProxy();

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
		catch (const exception& e)
		{
			_LOG(e.what());
		}

		return EXIT_FAILURE;
	}
};


void parse_args(int argc, char* argv[])
{
	namespace po_ = boost::program_options;

	po_::options_description visible("Options");
	visible.add_options()
		("master", po_::value<string>(), "master hostname")
		("storage", po_::value<string>(), "disk or memory")
		("help", "produce help message")
		;

	po_::options_description cmdline_opt;
	cmdline_opt.add(visible);

	po_::store(po_::command_line_parser(argc, argv).
			options(cmdline_opt).run(), povm);
	po_::notify(povm);

	if (povm.count("help"))
	{
		cout << visible << "\n";
		exit(EXIT_SUCCESS);
	}

	if (povm.count("master") == 0)
	{
		cout << "You need to specify a master.\n\n";
		cout << visible << "\n";
		exit(EXIT_FAILURE);
	}

	if (povm.count("storage") == 0)
	{
		cout << "You need to specify a storage.\n\n";
		cout << visible << "\n";
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char* argv[])
{
	int rc = EXIT_FAILURE;

	parse_args(argc, argv);

	cout << "server started.\n";
	cout << "  master: " << povm["master"].as<string>() << "\n";

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
	catch (const exception& e)
	{
		_LOG(e.what());
	}

	_LOG("server finished.");

	return rc;
}
