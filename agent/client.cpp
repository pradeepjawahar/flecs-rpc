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
		_servers.push_back(ServerPrx("flecs10"));
		_servers.push_back(ServerPrx("flecs20"));
		// _servers.push_back(ServerPrx("flecs40"));
		_servers.push_back(ServerPrx("flecs50"));
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
			if (i->_hostname == "flecs10")
			{
				string cmd = "touch /dev/shm/work/flecs-rpc/.build/launcher/flecs.trigger.master; ";
				i->BeginExec(cmd);
				i->EndExec();
			}
		}
	}


	void _StartServers()
	{
		_LOG("");

		string cmd = "touch /dev/shm/work/flecs-rpc/.build/launcher/flecs.trigger.server; ";

		for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
			i->BeginExec(cmd);

		for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
			i->EndExec();
	}


	void _StartClients()
	{
		_LOG("");

		string cmd = "touch /dev/shm/work/flecs-rpc/.build/launcher/flecs.trigger.client; ";

		for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
			if (i->_hostname == "flecs10")
				i->BeginExec(cmd);

		for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
			if (i->_hostname == "flecs10")
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
