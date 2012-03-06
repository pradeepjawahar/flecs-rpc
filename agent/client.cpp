#include <Ice/Ice.h>

#include "agent.h"
#include "agent-common.h"

using namespace std;

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("agent.client"));


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
			_LOG(("Initialized prx of ") + _hostname);
		}


		void BeginExec(const string& cmd)
		{
			_arp = _prx->begin_Exec(cmd);

			// TODO: read command from file. (:10) low priority.
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

			_LOG(_hostname + " " + log_filename);
		}
	};


public:
	AgentClient()
	{
	}


	virtual int run(int, char*[])
	{
		try
		{
			shutdownOnInterrupt();

			_init();

			_KillServices();
			_StartMaster();
			_StartServers();

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
		// TODO: load server list from file (:20) low priority.
		_servers.push_back(ServerPrx("polynesia1.cc.gatech.edu"));
		_servers.push_back(ServerPrx("polynesia2.cc.gatech.edu"));
		_servers.push_back(ServerPrx("polynesia4.cc.gatech.edu"));
		_servers.push_back(ServerPrx("polynesia5.cc.gatech.edu"));
	}


	void _KillServices()
	{
		_LOG("");

		string cmd = "killall -w flecs-server flecs-master flecs-client || true; "
			"killall -w -s KILL flecs-server flecs-master flecs-client || true; ";

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
			if (i->_hostname == "polynesia1.cc.gatech.edu")
			{
				// daemon is needed here. Even though flecs-master daemonize
				// itself, there agent-server seems to wait for it to finish.
				// There might be some resources that agent-server waits to be
				// released.
				string cmd = "cd /dev/shm/work/flecs-rpc/.build/master; daemon ./flecs-master; ";
				i->BeginExec(cmd);
				i->EndExec();
			}
		}
	}


	void _StartServers()
	{
		_LOG("");

		string cmd = "cd /dev/shm/work/flecs-rpc/.build/server; daemon ./flecs-server; ";

		for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
			i->BeginExec(cmd);

		for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
			i->EndExec();
	}


	vector<ServerPrx> _servers;
};


int main(int argc, char* argv[])
{
	int rc;

	try
	{
		_LOG("Client starting...");

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
