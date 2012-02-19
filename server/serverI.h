// **********************************************************************
//
// Copyright (c) 2003-2011 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef __SERVER_I_H__
#define __SERVER_I_H__

#include "flecs.h"


class ServerI : public FleCS::Server
{
public:
	virtual void Append(
			const std::string& objID,
			const FleCS::ByteSeq& content,
			const Ice::Current&);

	virtual void ServerJoined(
			const std::string& endpoint,
			const Ice::Current&);

	virtual void ServerLeft(
			const std::string& endpoint,
			const Ice::Current&);
};


#endif
