#include <fstream>
#include <string>
#include <vector>

#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <boost/program_options.hpp>

#include <Ice/Ice.h>

#include <log4cxx/logger.h>

#include "flecs.h"
#include "util.h"

#define _LOG(A) LOG4CXX_INFO(logger, A)

using namespace std;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("client"));

boost::program_options::variables_map povm;


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

			_RandomReadsAppends(0.1, 5000);

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
		_load_filelist();

		srandom(time(NULL));

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


	void _Put(const char* obj_name)
	{
		FleCS::ByteSeq content;

		_readfile(obj_name, content);

		_c2s_prx->Put(_bucket_name, obj_name, content);
	}


	void _Get(const char* obj_name)
	{
		_LOG(obj_name);

		FleCS::ByteSeq content;

		StopWatch sw;
		_c2s_prx->Get(_bucket_name, obj_name, content);
		unsigned int laps = sw.GetMicro();
		_LOG("G " << laps << " " << content.size());

		_writefile(obj_name, content);
	}


	// r_ratio (read_ratio) = r / r + w. ranges [0:1].
	void _RandomReadsAppends(
			const double r_ratio,
			const int runs)
	{
		for (int i = 0; i < runs; ++ i)
		{
			long r = random();
			char op = '-';	// 'R' or 'W'

			((double) r / RAND_MAX < r_ratio) ? op = 'R' : op = 'W';

			// pick an index of a file and filename.
			r = random();
			long idx = r % _filelist.size();
			string filename = _filelist[idx];

			switch (op)
			{
			case 'R':
				_Get(filename.c_str());
				break;

				// Note: Good to support append? FleCS would be then a file
				// store, not a key-value store. Or a key-value store with
				// append support.
			case 'W':
				_Append(filename);
				break;

			default:
				break;
			}
		}
	}
	
	
	void _Append(const string& filename)
	{
		_LOG(filename);

		static int seqno = 0;

		char sseqno[20];
		sprintf(sseqno, "%06d", seqno);
		++ seqno;

		// Generate random content
		FleCS::ByteSeq content;

		int cnt;
		if (povm["dist"].as<string>() == "zifian")
		{
			const long MIN = 100;
			const long MAX = 10000;
			cnt = _random(MIN, MAX);
		}
		else if (povm["dist"].as<string>() == "uniform")
			cnt = 1000;
		else
			throw runtime_error(string("Unknown dist: ") + povm["dist"].as<string>());

		for (int i = 0; i < cnt; i ++)
		{
			content.insert(content.end(), _hostname.c_str(), _hostname.c_str() + _hostname.size());
			content.push_back('.');
			content.insert(content.end(), sseqno, sseqno + 6);
			content.push_back(' ');
		}
		content.push_back('\n');

		StopWatch sw;
		_c2s_prx->Append(_bucket_name, filename, content);
		unsigned int laps = sw.GetMicro();
		_LOG("A " << laps << " " << content.size());
	}


	FleCS::C2SPrx _c2s_prx;
	vector<string> _filelist;
	string _hostname;

	static const char* _bucket_name;
};


const char* FleCSClient::_bucket_name = "rep-no-const";


void parse_args(int argc, char* argv[])
{
	namespace po_ = boost::program_options;

	po_::options_description visible("Options");
	visible.add_options()
		("dist", po_::value<string>(), "zifian or uniform")
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

	if (povm.count("dist") == 0)
	{
		cout << "You need to specify a distribution.\n\n";
		cout << visible << "\n";
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char* argv[])
{
	int rc = EXIT_FAILURE;

	parse_args(argc, argv);

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
