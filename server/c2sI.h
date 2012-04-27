#ifndef __C2S_I_H__
#define __C2S_I_H__

#include "flecs.h"

class C2SI : public FleCS::C2S
{
public:
    virtual void Get(
			const std::string& bucketID,
			const std::string& objID,
			FleCS::ByteSeq& content,
			const Ice::Current&);

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

    virtual void Process(
			const std::string& bucketID,
			const std::string& objID,
			const Ice::Current&);
};


#endif
