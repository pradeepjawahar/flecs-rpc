#include <log4cxx/logger.h>

#include <Ice/Ice.h>

#include "agent.h"

using namespace std;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("agent.client"));
#define _LOG(A) LOG4CXX_INFO(logger, (A))


class AgentClient : public Ice::Application
{
public:
	AgentClient() :
		Ice::Application(Ice::NoSignalHandling)
	{
	}


	virtual int run(int, char*[])
	{
		_init();

		int ret;
		string logFile;

		// _agent_prx->Exec("date; date;", ret, logFile);

		Ice::AsyncResultPtr asr = _agent_prx->begin_Exec("date; date;");

		// TODO: verify the commands are overlapping by sleeping 10 secs
		// between the two dates. (:10)

		// TODO: read command from file. (:10)
		// TODO: load server list from file (:20)

		_agent_prx->end_Exec(ret, logFile, asr);

		if (ret == -1)
			cout << "exec failed." << "\n";

		if (WIFSIGNALED(ret))
			cout << "signaled: " << WTERMSIG(ret) << "\n";

		if (WEXITSTATUS(ret) != 0)
			cout << "exit status: " << WEXITSTATUS(ret) << "\n";

		return EXIT_SUCCESS;
	}


private:
	void _init()
	{
		string endpoint = "agent:tcp -p 10010 -h polynesia2.cc.gatech.edu";

		// init RPC proxy
		_agent_prx = Common::AgentPrx::checkedCast(
				communicator()
				->stringToProxy(endpoint)
				->ice_twoway()
				->ice_timeout(-1)
				->ice_secure(false));
		if(!_agent_prx)
		{
			_LOG("invalid proxy");
			exit(EXIT_FAILURE);
		}

		_LOG("Initialized.");
	}


	Common::AgentPrx _agent_prx;
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
