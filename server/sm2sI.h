#ifndef __SERVER_I_H__
#define __SERVER_I_H__

#include "flecs.h"


class SM2SI : public FleCS::SM2S
{
public:
	virtual void Put(
			const std::string& bucketID,
			const std::string& objID,
			const FleCS::ByteSeq& content,
			const Ice::Current&);

	virtual void Append(
			const std::string& bucketID,
			const std::string& objID,
			const FleCS::ByteSeq& content,
			const Ice::Current&);

	virtual void ServerJoined(
			const std::string& endpoint,
			const Ice::Current&);

	virtual void ServerLeft(
			const std::string& endpoint,
			const Ice::Current&);

	virtual int AcquireLock(
			const std::string& lockPath,
			Ice::Byte type,
			const Ice::Current&);

	virtual void ReleaseLock(
			int lockID,
			const Ice::Current&);
};


#endif
