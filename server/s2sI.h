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


class S2SI : public FleCS::S2S
{
public:
	virtual void Append(
			const std::string& objID,
			const FleCS::ByteSeq& content,
			const Ice::Current&);
};


#endif
