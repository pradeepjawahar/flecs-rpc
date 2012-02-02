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
	sequence<byte> ByteSeq;

	interface C2S
	{
		// TODO: Add version as a parameter. A simple scalar value will do? or vector?

		void Get(string objID, out ByteSeq content);
		void Put(string objID, ByteSeq content);
	};


	interface S2S
	{
		int add(int a, int b, int c);
	};


};

#endif
