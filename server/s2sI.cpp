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
#include "util.h"
#include "server-common.h"

using namespace std;


void S2SI::Append(
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	cout << "S2S::Append(" << objID << ")\n";

	_appendfile((string(FleCS::Server::stg_root_dir) + "/" + objID).c_str(), content);
}
