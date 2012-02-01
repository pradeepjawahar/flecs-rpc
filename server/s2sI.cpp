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
#include "s2sI.h"

using namespace std;


int S2SI::add(int a, int b, int c, const Ice::Current&)
{
	return a + b + c;
}
