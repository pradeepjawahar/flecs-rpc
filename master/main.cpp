#include <Ice/Ice.h>

#include <log4cxx/logger.h>

#include "flecs.h"
#include "masterI.h"

using namespace std;

#define _LOG(A) LOG4CXX_INFO(logger, A)

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("master"));


class FleCSMaster : public Ice::Application
{
public:
	virtual int run(int, char*[])
	{
		try
		{
			shutdownOnInterrupt();

			Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("FleCS");

			FleCS::MasterPtr m = new MasterI;
			adapter->add(m, communicator()->stringToIdentity("master"));

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
	int rc;

	cout << "master started.\n";

	if (daemon(1, 0) == -1)
	{
		perror("daemon");
		return EXIT_FAILURE;
	}

	_LOG("Master starting...");

	try
	{
		FleCSMaster app;
		rc = app.main(argc, argv, "config.master");
	}
	catch(const exception& e)
	{
		_LOG(e.what());
	}

	_LOG("Master finished.");

	return rc;
}
