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

#include "serverI.h"
#include "util.h"
#include "server-common.h"

using namespace std;


void ServerI::Append(
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	cout << "S2S::Append(" << objID << ")\n";

	_appendfile((string(FleCS::ServerImpl::stg_root_dir) + "/" + objID).c_str(), content);
}


void ServerI::ServerJoined(
		const std::string& endpoint,
		const Ice::Current& cur)
{
	cout << __FUNCTION__ << "(" << endpoint << ")\n";

	FleCS::ServerImpl::AddServer(endpoint, cur.adapter->getCommunicator());
}


void ServerI::ServerLeft(
		const std::string& endpoint,
		const Ice::Current&)
{
	cout << __FUNCTION__ << "(" << endpoint << ")\n";
}
