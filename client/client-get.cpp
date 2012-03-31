#include <Ice/Ice.h>

#include <log4cxx/logger.h>

#include "flecs.h"
#include "util.h"

#define _LOG(A) LOG4CXX_INFO(logger, A)

using namespace std;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("client"));


class FleCSClient : public Ice::Application
{
public:
    FleCSClient() :
		Ice::Application(Ice::NoSignalHandling)
	{
	}


	virtual int run(int argc, char* argv[])
	{
		try
		{
			_init();

			FleCS::ByteSeq content;

			_c2s_prx->Get("00000001/00000001/00000001", content);

			_writefile("abc", content);

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
		// init RPC proxy
		_c2s_prx = FleCS::C2SPrx::checkedCast(
				communicator()
				->propertyToProxy("FleCS_c2s.Proxy")
				->ice_twoway()
				->ice_timeout(-1)
				->ice_secure(false));
		if(!_c2s_prx)
		{
			_LOG("invalid proxy");
			exit(EXIT_FAILURE);
		}

		_LOG("Initialized.");
	}


	FleCS::C2SPrx _c2s_prx;
};


int main(int argc, char* argv[])
{
	int rc;

	cout << "client started.\n";

	if (daemon(1, 0) == -1)
	{
		perror("daemon");
		return EXIT_FAILURE;
	}

	_LOG("Client starting...");

	try
	{
		FleCSClient app;
		rc = app.main(argc, argv, "config.client");
	}
	catch(const exception& e)
	{
		_LOG(e.what());
	}

	_LOG("Client finished.");

	return rc;
}
