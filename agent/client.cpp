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
		Ice::AsyncResultPtr asr;

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
			_LOG(("Initialized prx to ") + _hostname);
		}


		void BeginExec()
		{
			asr = _prx->begin_Exec("date; sleep 2; date;");

			// TODO: read command from file. (:10) low priority.
		}


		void EndExec()
		{
			int ret;
			string log_filename;

			_prx->end_Exec(ret, log_filename, asr);

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
	AgentClient() :
		Ice::Application(Ice::NoSignalHandling)
	{
	}


	virtual int run(int, char*[])
	{
		try
		{
			_init();

			for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
				i->BeginExec();

			for (vector<ServerPrx>::iterator i = _servers.begin(); i != _servers.end(); ++ i)
				i->EndExec();

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
