#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>

#include "c2sI.h"
#include "util.h"
#include "server-common.h"
#include "container.h"

using namespace std;


// TODO: these two classes will go to a container-specific code.

class PutThread : public IceUtil::Thread
{
public:
	PutThread(
			const FleCS::ServerPrx& s,
			const string& bucketID,
			const string& objID,
			const FleCS::ByteSeq& content)
		: _s(s), _bucketID(bucketID), _objID(objID), _content(content)
	{
	}

	virtual void run()
	{
		_s->Put(_bucketID, _objID, _content);
	}


private:
	const FleCS::ServerPrx& _s;
	const string& _bucketID;
	const string& _objID;
	const FleCS::ByteSeq& _content;
};


class AppendThread : public IceUtil::Thread
{
public:
	AppendThread(
			const FleCS::ServerPrx& s,
			const string& bucketID,
			const string& objID,
			const FleCS::ByteSeq& content)
		: _s(s), _bucketID(bucketID), _objID(objID), _content(content)
	{
	}

	virtual void run()
	{
		_s->Append(_bucketID, _objID, _content);
	}


private:
	const FleCS::ServerPrx& _s;
	const string& _bucketID;
	const string& _objID;
	const FleCS::ByteSeq& _content;
};


void C2SI::Get(
		const std::string& bucketID,
		const std::string& objID,
		FleCS::ByteSeq& content,
		const Ice::Current&)
{
	// Depending on the bucketID, a proper container code needs to be invoked.
	// For quick prototyping, for now, there is only one bucket (instance of a
	// container) per container. It can be easily extended to allow multiple
	// buckets per container with like this:
	// /storage_root/container_type/bucket_name/obj_path/...
	
	static ContainerMgr& cm = ContainerMgr::GetInstance();
	Container& c = cm.GetContainer(bucketID);

	c.C2S_Get(bucketID, objID, content);
}


void C2SI::Put(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	_LOG(objID);

	_writefile((string(FleCS::ServerImpl::stg_root_dir) + "/" + bucketID + "/" + objID).c_str(), content);

	// propagate update to the other servers.

	map<string, FleCS::ServerPrx*>& s = FleCS::ServerImpl::_servers;

#ifdef _SERIAL_PROCESSING
	for (map<string, FleCS::ServerPrx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
		(*(i->second))->Put(objID, content);

#else	// Parallel processing (by default)
	vector<IceUtil::ThreadPtr> tpv;
	vector<IceUtil::ThreadControl> tcv;

	for (map<string, FleCS::ServerPrx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
	{
		IceUtil::ThreadPtr t = new PutThread(*(i->second), bucketID, objID, content);
		tcv.push_back(t->start());
		tpv.push_back(t);
	}

	for (vector<IceUtil::ThreadControl>::iterator j = tcv.begin(); j != tcv.end(); ++ j)
		j->join();
#endif
}


void C2SI::Append(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content,
		const Ice::Current&)
{
	_LOG(objID);

	_appendfile((string(FleCS::ServerImpl::stg_root_dir) + "/" + bucketID + "/" + objID).c_str(), content);

	// propagate update to the other servers.
	//
	// If there is only one thread that acts as dispatcher and worker, this may cause a deadlock.
	//   C1 ----> S1 --(S2 is not responding while processing C2's request)--> S2
	//   C2 ----> S2 --(S1 is not responding while processing C1's request)--> S1
	//
	// The size of thread pool seems to be large enough, thus the above case
	// won't happen. But it happens!, which means (at least by default) the
	// dispatcher and workers are, in some way, closely related. It applies the
	// same to Put() method.
	map<string, FleCS::ServerPrx*>& s = FleCS::ServerImpl::_servers;

#ifdef _SERIAL_PROCESSING
	for (map<string, FleCS::ServerPrx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
		(*(i->second))->Append(objID, content);

#else	// Parallel processing (by default)
	vector<IceUtil::ThreadPtr> tpv;
	vector<IceUtil::ThreadControl> tcv;

	for (map<string, FleCS::ServerPrx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
	{
		IceUtil::ThreadPtr t = new AppendThread(*(i->second), bucketID, objID, content);
		tcv.push_back(t->start());
		tpv.push_back(t);
	}

	for (vector<IceUtil::ThreadControl>::iterator j = tcv.begin(); j != tcv.end(); ++ j)
		j->join();
#endif
}
