#include <fstream>
#include <string>
#include <vector>

#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/ip/host_name.hpp>

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
		_hostname = boost::asio::ip::host_name();
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


	void _load_filelist()
	{
		const char* filename = "/usr/local/flecs/no-cnst-filelist";

		ifstream file(filename, ios::in);

		if (! file.is_open())
		{
			_LOG("Unable to open file " << filename);
			exit(EXIT_FAILURE);
		}

		while (file.good())
		{
			string line;
			getline(file, line);

			if (! file.eof())
			{
				typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
				boost::char_separator<char> sep(" ");
				tokenizer tokens(line, sep);

				string filename;

				int j = 0;
				for (tokenizer::iterator i = tokens.begin(); i != tokens.end(); ++ i, ++ j)
				{
					if (j == 1)
						filename = *i;
				}

				_filelist.push_back(filename);
			}
		}

		file.close();
	}


	FleCS::C2SPrx _c2s_prx;
	vector<string> _filelist;
	string _hostname;
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
