// **********************************************************************
//
// Copyright (c) 2003-2011 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <flecs.h>

using namespace std;
using namespace FleCS;

class HelloClient : public Ice::Application
{
public:
    HelloClient();

    virtual int run(int, char*[]);

private:
};


HelloClient::HelloClient() :
    //
    // Since this is an interactive demo we don't want any signal
    // handling.
    //
    Ice::Application(Ice::NoSignalHandling)
{
}


int HelloClient::run(int argc, char* argv[])
{
	try
	{
		C2SPrx c2s_prx = C2SPrx::checkedCast(
				communicator()
				->propertyToProxy("FleCS_c2s.Proxy")
				->ice_twoway()
				->ice_timeout(-1)
				->ice_secure(false));
		if(!c2s_prx)
		{
			cerr << argv[0] << ": invalid proxy" << endl;
			return EXIT_FAILURE;
		}

		S2SPrx s2s_prx = S2SPrx::checkedCast(
				communicator()
				->propertyToProxy("FleCS_s2s.Proxy")
				->ice_twoway()
				->ice_timeout(-1)
				->ice_secure(false));
		if(!s2s_prx)
		{
			cerr << argv[0] << ": invalid proxy" << endl;
			return EXIT_FAILURE;
		}

		/*
		int timeout = 1000;

		c2s_prx = c2s_prx->ice_timeout(timeout);

		if (timeout == -1)
			cout << "timeout is now switched off" << endl;
		else
			cout << "timeout is now set to " << timeout << "\n";
		*/

		c2s_prx->sayHello();

		int c = c2s_prx->add(4, 5);
		cout << "c=" << c << "\n";

		cout << "s2s_prx->add()=" << s2s_prx->add(4, 5, 6) << "\n";

		//c2s_prx->shutdown();
	}
	catch(const Ice::Exception& ex)
	{
		cerr << ex << endl;
	}

    return EXIT_SUCCESS;
}


int main(int argc, char* argv[])
{
    HelloClient app;
    return app.main(argc, argv, "config.client");
}


