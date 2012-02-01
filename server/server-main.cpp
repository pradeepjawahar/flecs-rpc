// **********************************************************************
//
// Copyright (c) 2003-2011 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <Ice/Ice.h>
#include "c2sI.h"
#include "s2sI.h"

using namespace std;

class HelloServer : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};

int
main(int argc, char* argv[])
{
    HelloServer app;
    return app.main(argc, argv, "config.server");
}

int
HelloServer::run(int argc, char* argv[])
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
