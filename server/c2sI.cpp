// **********************************************************************
//
// Copyright (c) 2003-2011 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include "c2sI.h"

using namespace std;

void C2SI::sayHello(const Ice::Current&)
{
	//IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(500));
	
    cout << "Hello World!" << endl;
}

int C2SI::add(int a, int b, const Ice::Current&)
{
	return a + b;
}

void C2SI::shutdown(const Ice::Current& c)
{
    cout << "Shutting down..." << endl;
    c.adapter->getCommunicator()->shutdown();
}
