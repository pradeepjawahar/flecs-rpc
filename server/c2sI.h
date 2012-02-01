// **********************************************************************
//
// Copyright (c) 2003-2011 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef __C2S_I_H__
#define __C2S_I_H__

#include "flecs.h"

class C2SI : public FleCS::C2S
{
public:
    virtual void sayHello(const Ice::Current&);
	virtual int add(int a, int b, const Ice::Current&);
    virtual void shutdown(const Ice::Current&);
};


#endif
