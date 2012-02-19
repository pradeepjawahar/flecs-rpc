// **********************************************************************
//
// Copyright (c) 2003-2011 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef __S2S_I_H__
#define __S2S_I_H__

#include "flecs.h"


class ServerI : public FleCS::Server1
{
public:
	virtual void Append(
			const std::string& objID,
			const FleCS::ByteSeq& content,
			const Ice::Current&);

	virtual void ServerAdded(
			const std::string& endpoint,
			const Ice::Current&);

	virtual void ServerRemoved(
			const std::string& endpoint,
			const Ice::Current&);
};


#endif
