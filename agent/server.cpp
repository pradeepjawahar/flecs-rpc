#include <Ice/Ice.h>

#include "agent-common.h"
#include "agentI.h"

using namespace std;


log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("agent.server"));


class AgentServer : public Ice::Application
{
public:
    virtual int run(int, char*[])
	{
		try
		{
			shutdownOnInterrupt();

			Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Agent");

			Common::AgentPtr agent = new AgentI;
			adapter->add(agent, communicator()->stringToIdentity("agent"));
			adapter->activate();

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


int main(int argc, char* argv[])
{
	int rc = EXIT_FAILURE;

	cout << "server started.\n";

	if (daemon(1, 0) == -1)
	{
		perror("daemon");
		return EXIT_FAILURE;
	}

	_LOG("agent server starting...");

	try
	{
		AgentServer app;
		rc = app.main(argc, argv, "config.agent.server");
	}
	catch (const exception& e)
	{
		_LOG(e.what());
	}

	_LOG("agent server finished.");

	return rc;
}
