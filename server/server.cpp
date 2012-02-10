#include "server-common.h"
#include "server.h"
#include "util.h"

#include "c2sI.h"
#include "s2sI.h"

using namespace std;


FleCSServer::FleCSServer()
{
	// create server root dir
	_create_directories(FleCS::Server::stg_root_dir);
}


int FleCSServer::run(int argc, char* argv[])
{
    Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("FleCS");

    FleCS::C2SPtr c2s = new C2SI;
    adapter->add(c2s, communicator()->stringToIdentity("c2s"));

    FleCS::S2SPtr s2s = new S2SI;
    adapter->add(s2s, communicator()->stringToIdentity("s2s"));

    adapter->activate();
    communicator()->waitForShutdown();

	cout << "Serverr closing.\n";
    return EXIT_SUCCESS;
}
