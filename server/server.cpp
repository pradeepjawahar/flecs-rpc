#include "server.h"
#include "util.h"
#include "c2sI.h"
#include "sm2sI.h"

using namespace std;


boost::program_options::variables_map povm;

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("server"));


FleCS::SM2SPrx* _new_sm2s_proxy(
		const string& endpoint)
{
	FleCS::SM2SPrx* s_prx = new FleCS::SM2SPrx;

	*s_prx = FleCS::SM2SPrx::checkedCast(
			Ice::Application::communicator()
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


FleCSServer::FleCSServer()
{
	if (povm["storage"].as<string>() == "disk")
		stg_root_dir = "/usr/local/flecs/data";
	else if (povm["storage"].as<string>() == "memory")
		stg_root_dir = "/dev/shm/flecs/data";
	else
		throw runtime_error(string("Unknown storage: ") + stg_root_dir);

	// create server root dir
	_create_directories(stg_root_dir);
}


FleCSServer::~FleCSServer()
{
	// Delete proxies to other servers.
	for (map<std::string, FleCS::SM2SPrx*>::iterator i = peer_servers.begin(); i != peer_servers.end(); ++ i)
		delete i->second;
}


int FleCSServer::run(int, char*[])
{
	try
	{
		shutdownOnInterrupt();

		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("FleCS");

		FleCS::C2SPtr c2s = new C2SI;
		adapter->add(c2s, communicator()->stringToIdentity("c2s"));

		FleCS::SM2SPtr sm2s = new SM2SI;
		adapter->add(sm2s, communicator()->stringToIdentity("sm2s"));

		adapter->activate();

		// Notify master that a server is on.
		//
		// No deadlock as long as the master do not notify the originating server.
		//   server --(join)--> master --(notify)--> all other servers.
		//
		// Master needs to make sure that join service is serialized.

		FleCS::MasterPrx& m_prx = GetMasterProxy();

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
		AddServers(existingServers);

		communicator()->waitForShutdown();

		return EXIT_SUCCESS;
	}
	catch (const exception& e)
	{
		_LOG(e.what());
	}

	return EXIT_FAILURE;
}


void FleCSServer::AddServers(
		const vector<string>& servers)
{
	for (vector<string>::const_iterator i = servers.begin(); i != servers.end(); ++ i)
	{
		if (i == servers.begin())
			_LOG("AddServers:");

		AddServer(*i);
		_LOG("  " <<  *i);
	}
}


void FleCSServer::AddServer(
		const string& endpoint)
{
	// check if the endpoint already exists.
	map<string, FleCS::SM2SPrx*>::iterator i = peer_servers.find(endpoint);

	if (i != peer_servers.end())
	{
		_LOG("A server with endpoint " << endpoint << " already exist! Replacing with a new one.");

		// remove old proxy.
		delete i->second;
	}

	peer_servers[endpoint] = _new_sm2s_proxy(endpoint);
}


FleCS::MasterPrx& FleCSServer::GetMasterProxy()
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


std::string FleCSServer::stg_root_dir;

std::map<std::string, FleCS::SM2SPrx*> FleCSServer::peer_servers;


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
