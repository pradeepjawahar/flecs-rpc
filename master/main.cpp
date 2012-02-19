#include <iostream>
#include <Ice/Ice.h>

#include "flecs.h"
#include "masterI.h"

using namespace std;


class FleCSMaster : public Ice::Application
{
public:
	virtual int run(int, char*[])
	{
		shutdownOnInterrupt();

		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("FleCS");

		FleCS::MasterPtr m = new MasterI;
		adapter->add(m, communicator()->stringToIdentity("master"));

		adapter->activate();
		communicator()->waitForShutdown();

		cout << "Serverr closing.\n";
		return EXIT_SUCCESS;
	}
};


int main(int argc, char* argv[])
{
    FleCSMaster app;
    return app.main(argc, argv, "config.master");
}
