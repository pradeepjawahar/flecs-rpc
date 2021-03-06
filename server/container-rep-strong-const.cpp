#include "container.h"
#include "util.h"
#include "server.h"
#include "globallock.h"

using namespace std;


class PutThread : public IceUtil::Thread
{
public:
	PutThread(
			const FleCS::SM2SPrx& s,
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
	const FleCS::SM2SPrx& _s;
	const string& _bucketID;
	const string& _objID;
	const FleCS::ByteSeq& _content;
};


class AppendThread : public IceUtil::Thread
{
public:
	AppendThread(
			const FleCS::SM2SPrx& s,
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
	const FleCS::SM2SPrx& _s;
	const string& _bucketID;
	const string& _objID;
	const FleCS::ByteSeq& _content;
};


class rep_strong_const : public Container
{
public:
	void Init()
	{
		_LOG("");
	}
	

	void C2S_Get(
		const std::string& bucketID,
		const std::string& objID,
		FleCS::ByteSeq& content)
	{
		_LOG(objID);

		string boID = bucketID + "/" + objID;

		GlobalLock lock(boID, 'R');

		_readfile((string(FleCSServer::stg_root_dir) + "/" + boID).c_str(), content);
	}
	

	void C2S_Put(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content)
	{
		_LOG(objID);

		string boID = bucketID + "/" + objID;

		GlobalLock lock(boID, 'W');

		_writefile((string(FleCSServer::stg_root_dir) + "/" + boID).c_str(), content);

		// propagate update to the other servers.

		map<string, FleCS::SM2SPrx*>& s = FleCSServer::peer_servers;

#ifdef _SERIAL_PROCESSING
		for (map<string, FleCS::SM2SPrx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
			(*(i->second))->Put(bucketID, objID, content);

#else	// Parallel processing (by default)
		vector<IceUtil::ThreadPtr> tpv;
		vector<IceUtil::ThreadControl> tcv;

		for (map<string, FleCS::SM2SPrx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
		{
			IceUtil::ThreadPtr t = new PutThread(*(i->second), bucketID, objID, content);
			tcv.push_back(t->start());
			tpv.push_back(t);
		}

		for (vector<IceUtil::ThreadControl>::iterator j = tcv.begin(); j != tcv.end(); ++ j)
			j->join();
#endif
	}


	void C2S_Append(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content)
	{
		_LOG(objID);

		string boID = bucketID + "/" + objID;

		GlobalLock lock(boID, 'W');

		_appendfile((string(FleCSServer::stg_root_dir) + "/" + bucketID + "/" + objID).c_str(), content);

		// propagate update to the other servers.
		//
		// You need to make sure that a deadlock does not happen, such as:
		//   C1 ----> S1 --(S2 is not responding while processing C2's request)--> S2
		//   C2 ----> S2 --(S1 is not responding while processing C1's request)--> S1
		//
		// It can be avoided by serializing requests from a connection and
		// setting the thread pool size large enough. At least to the (number
		// of local clients) * (number of servers).
		//
		// It applies the same to the Put().
		map<string, FleCS::SM2SPrx*>& s = FleCSServer::peer_servers;

#ifdef _SERIAL_PROCESSING
		for (map<string, FleCS::SM2SPrx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
			(*(i->second))->Append(bucketID, objID, content);

#else	// Parallel processing (by default)
		vector<IceUtil::ThreadPtr> tpv;
		vector<IceUtil::ThreadControl> tcv;

		for (map<string, FleCS::SM2SPrx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
		{
			IceUtil::ThreadPtr t = new AppendThread(*(i->second), bucketID, objID, content);
			tcv.push_back(t->start());
			tpv.push_back(t);
		}

		for (vector<IceUtil::ThreadControl>::iterator j = tcv.begin(); j != tcv.end(); ++ j)
			j->join();
#endif
	}


	// This function is called with a 'W' lock held.
	void S2S_Put(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content)
	{
		_LOG(objID);

		_writefile((string(FleCSServer::stg_root_dir) + "/" + bucketID + "/" + objID).c_str(), content);
	}
	
	
	// This function is called with a 'W' lock held.
	void S2S_Append(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content)
	{
		_LOG(objID);

		_appendfile((string(FleCSServer::stg_root_dir) + "/" + bucketID + "/" + objID).c_str(), content);
	}


private:
};


extern "C" rep_strong_const* CreateInstance()
{
	return new rep_strong_const;
}
