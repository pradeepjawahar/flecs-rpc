#include <Ice/Ice.h>
#include <flecs.h>
#include "util.h"

using namespace std;
using namespace FleCS;

class FleCSClient : public Ice::Application
{
public:
    FleCSClient();

    virtual int run(int, char*[]);

private:
	void _Init();
	void _Put(const char* obj_name);
	void _Get(const char* obj_name);

	C2SPrx _c2s_prx;
	S2SPrx _s2s_prx;
};


FleCSClient::FleCSClient() :
    //
    // Since this is an interactive demo we don't want any signal
    // handling.
    //
    Ice::Application(Ice::NoSignalHandling)
{
}


int FleCSClient::run(int argc, char* argv[])
{
	try
	{
		_Init();

		_Put("config.client");

		_Get("config.server");

		// TODO: _GenFiles();

		// TODO: _RunFilebench();
		// with strongly consistent model.

		//cout << "s2s_prx->add()=" << s2s_prx->add(4, 5, 6) << "\n";
	}
	catch(const Ice::Exception& ex)
	{
		cerr << ex << endl;
	}

    return EXIT_SUCCESS;
}


void FleCSClient::_Init()
{
	_c2s_prx = C2SPrx::checkedCast(
			communicator()
			->propertyToProxy("FleCS_c2s.Proxy")
			->ice_twoway()
			->ice_timeout(-1)
			->ice_secure(false));
	if(!_c2s_prx)
	{
		cerr << "invalid proxy" << endl;
		exit(EXIT_FAILURE);
	}

	_s2s_prx = S2SPrx::checkedCast(
			communicator()
			->propertyToProxy("FleCS_s2s.Proxy")
			->ice_twoway()
			->ice_timeout(-1)
			->ice_secure(false));
	if(!_s2s_prx)
	{
		cerr << "invalid proxy" << endl;
		exit(EXIT_FAILURE);
	}
}


void FleCSClient::_Put(const char* obj_name)
{
	ByteSeq content;

	_readfile(obj_name, content);

	_c2s_prx->Put(obj_name, content);
}


void FleCSClient::_Get(const char* obj_name)
{
	ByteSeq content;
	_c2s_prx->Get(obj_name, content);
}


int main(int argc, char* argv[])
{
    FleCSClient app;
    return app.main(argc, argv, "config.client");
}


