#include "container.h"
#include "util.h"

using namespace std;


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


class rep_no_const : public Container
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

		_readfile((string(FleCS::ServerImpl::stg_root_dir) + "/" + bucketID + "/" + objID).c_str(), content);
	}
	

	void C2S_Put(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content)
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


	void C2S_Append(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content)
	{
		_LOG(objID);

		_appendfile((string(FleCS::ServerImpl::stg_root_dir) + "/" + bucketID + "/" + objID).c_str(), content);

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


	void Server_Put(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content)
	{
		_LOG(objID);

		_writefile((string(FleCS::ServerImpl::stg_root_dir) + "/" + bucketID + "/" + objID).c_str(), content);
	}
	
	
	void Server_Append(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content)
	{
		_LOG(objID);

		_appendfile((string(FleCS::ServerImpl::stg_root_dir) + "/" + bucketID + "/" + objID).c_str(), content);
	}


private:
};


extern "C" rep_no_const* CreateInstance()
{
	return new rep_no_const;
}
