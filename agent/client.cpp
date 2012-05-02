#include <boost/program_options.hpp>
#include <Ice/Ice.h>

#include "agent.h"
#include "agent-common.h"

using namespace std;

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("agent.client"));

boost::program_options::variables_map povm;


class AgentClient : public Ice::Application
{
public:
	struct ServerPrx
	{
		string _hostname;
		string _endpoint;
		Common::AgentPrx _prx;
		Ice::AsyncResultPtr _arp;

		ServerPrx(const string& h)
			: _hostname(h)
		{
			_endpoint = string("agent:tcp -p 10010 -h ") + _hostname;

			// init RPC proxy
			_prx = Common::AgentPrx::checkedCast(
					communicator()
					->stringToProxy(_endpoint)
					->ice_twoway()
					->ice_timeout(-1)
					->ice_secure(false));
			if(!_prx)
			{
				_LOG("invalid proxy");
				exit(EXIT_FAILURE);
			}
		}


		void BeginExec(const string& cmd)
		{
			_arp = _prx->begin_Exec(cmd);
		}


		void EndExec()
		{
			int ret;
			string log_filename;

			_prx->end_Exec(ret, log_filename, _arp);

			if (ret == -1)
				_LOG("exec failed.");
			if (WIFSIGNALED(ret))
				_LOG("signaled: " << WTERMSIG(ret));
			if (WEXITSTATUS(ret) != 0)
				_LOG("exit status: " << WEXITSTATUS(ret));

			_LOG(_hostname << " " << log_filename);
		}
	};


public:
	AgentClient()
	{
	}


	virtual int run(int argc, char* argv[])
	{
		try
		{
			shutdownOnInterrupt();

			_init();

			_KillServices();
			_StartMaster();
			sleep(1);
			_StartServers();
			sleep(1);
			_StartClients();

			return EXIT_SUCCESS;
		}
		catch (const exception& e)
		{
			_LOG(e.what());
		}

		return EXIT_FAILURE;
	}


private:
	void _init()
	{
		if (povm["cluster"].as<string>() == "polymoa")
		{
			_servers.push_back(ServerPrx("polynesia1.cc.gatech.edu"));
			_servers.push_back(ServerPrx("polynesia2.cc.gatech.edu"));
			_servers.push_back(ServerPrx("polynesia5.cc.gatech.edu"));
			_servers.push_back(ServerPrx("samoa5.cc.gatech.edu"));
			_servers.push_back(ServerPrx("samoa6.cc.gatech.edu"));
			_servers.push_back(ServerPrx("samoa7.cc.gatech.edu"));
		}
		else if (povm["cluster"].as<string>() == "flecsx0")
		{
			_servers.push_back(ServerPrx("flecs10"));
			_servers.push_back(ServerPrx("flecs20"));
			_servers.push_back(ServerPrx("flecs50"));
		}
	}


	void _KillServices()
	{
		_LOG("");

		string cmd = "killall -w flecs-server flecs-master flecs-client* || true; "
			"killall -w -s KILL flecs-server flecs-master flecs-client* || true; ";

		for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
			i->BeginExec(cmd);

		for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
			i->EndExec();
	}


	void _StartMaster()
	{
		_LOG("");

		for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
		{
			if ((povm["cluster"].as<string>() == "polymoa"
					&& i->_hostname == "polynesia1.cc.gatech.edu")
					|| (povm["cluster"].as<string>() == "flecsx0"
					&& i->_hostname == "flecs10"))
			{
				string cmd = "touch /dev/shm/flecs/src/.build/launcher/flecs.trigger.master; ";
				i->BeginExec(cmd);
				i->EndExec();
			}
		}
	}


	void _StartServers()
	{
		_LOG("");

		string cmd = "touch /dev/shm/flecs/src/.build/launcher/flecs.trigger.server; ";

		for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
			i->BeginExec(cmd);

		for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
			i->EndExec();
	}


	void _StartClients()
	{
		_LOG("");

		string cmd = "touch /dev/shm/flecs/src/.build/launcher/flecs.trigger.client; ";

		if (povm["client_on"].as<string>() == "all")
		{
			for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
				i->BeginExec(cmd);

			for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
				i->EndExec();
		}
		else if (povm["client_on"].as<string>() == "first")
		{
			_servers.begin()->BeginExec(cmd);
			_servers.begin()->EndExec();
		}
		else
			throw runtime_error(string("Unknown client_on: ") + povm["client_on"].as<string>());
	}


	vector<ServerPrx> _servers;
};


void parse_args(int argc, char* argv[])
{
	namespace po_ = boost::program_options;

	po_::options_description visible("Options");
	visible.add_options()
		("cluster", po_::value<string>(), "e.g., polymoa or flecsx0")
		("client_on", po_::value<string>()->default_value("all"), "all (run client on every node) or first (only on the first node)")
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

	if (povm.count("cluster") == 0)
	{
		cout << "You need to specify a cluster.\n\n";
		cout << visible << "\n";
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char* argv[])
{
	int rc = EXIT_FAILURE;

	parse_args(argc, argv);

	try
	{
		_LOG("Client starting...");
		_LOG("  cluster: " << povm["cluster"].as<string>());

		AgentClient app;
		rc = app.main(argc, argv, "config.agent.client");

		_LOG("Client finished.");
	}
	catch(const exception& e)
	{
		_LOG(e.what());
	}

	return rc;
}
