#ifndef __SERVER_I_H__
#define __SERVER_I_H__

#include "flecs.h"


class ServerI : public FleCS::Server
{
public:
	virtual void Put(
			const std::string& objID,
			const FleCS::ByteSeq& content,
			const Ice::Current&);

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
