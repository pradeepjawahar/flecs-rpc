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

			vector<string> buckets;
			buckets.push_back("rep-strong-const");
			buckets.push_back("rep-no-const");

			string obj_name = "abc/def";
			FleCS::ByteSeq content;
			const char* randstr = "abcdefg1234567890";
			content.insert(content.end(), randstr, randstr + strlen(randstr));

			// put, get, delete, and get. With a single client, the last get is
			// expected to fail.  With multiple clients, the second get could
			// also fail, since the object may have been already deleted by
			// another client.
			for (vector<string>::iterator b = buckets.begin(); b != buckets.end(); ++ b)
			{
				_LOG("bucket: " << *b);

				_c2s_prx->Put(*b, obj_name, content);
				_LOG(" Put succeeded.");

				long size = _c2s_prx->Size(*b, obj_name);
				_LOG(" Size: " << size);

				_c2s_prx->Get(*b, obj_name, content);
				_LOG(" Get succeeded.");

				_c2s_prx->Delete(*b, obj_name);
				_LOG(" Delete succeeded");

				bool exception_thrown = false;

				try
				{
					_c2s_prx->Get(*b, obj_name, content);
				}
				catch(const Ice::Exception& e)
				{
					exception_thrown = true;
					_LOG(" Get failed.");
				}

				if (!exception_thrown)
					throw runtime_error("Get succeeded?? Expected to fail!!");
			}

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
	int rc = EXIT_FAILURE;

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
