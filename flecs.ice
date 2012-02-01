// **********************************************************************
//
// Copyright (c) 2003-2011 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef __FLECS_ICE__
#define __FLECS_ICE__

module FleCS
{

interface C2S
{
    void sayHello();
    int add(int a, int b);
    void shutdown();
};


interface S2S
{
    int add(int a, int b, int c);
};


};

#endif
