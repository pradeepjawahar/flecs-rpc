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

	virtual void Delete(
			const std::string& bucketID,
			const std::string& objID,
			const Ice::Current&);

	virtual void ServerJoined(
			const std::string& endpoint,
			const Ice::Current&);

	virtual void ServerLeft(
			const std::string& endpoint,
			const Ice::Current&);

	virtual void AcquireLock(
			const std::string& lockPath,
			Ice::Byte type,
			const std::string& hostname,
			long tid,
			const Ice::Current&);

	virtual void ReleaseLock(
			const std::string& lockPath,
			Ice::Byte type,
			const std::string& hostname,
			long tid,
			const Ice::Current&);

private:

	struct Mutex
	{
		Mutex()
			: ref_cnt(0)
		{
		}

		IceUtil::Mutex lock;
		int ref_cnt;
	};


	IceUtil::Mutex _locks_lock;

	std::map<std::string, Mutex*> _locks;
};


#endif
