#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/ip/host_name.hpp>
#include "container.h"
#include "util.h"
#include "server.h"
#include "globallock.h"
#include "hiredis/hiredis.h"
#include <cstdlib>
#include <cmath>
#include <ctime>
#define REDIS_HOST flecs52 
using namespace std;

class GetThread : public IceUtil::Thread
{
public:
        GetThread(
                        const FleCS::SM2SPrx& s,
                        const string& bucketID,
                        const string& objID,
                        FleCS::ByteSeq& serv_content)
                : _s(s), _bucketID(bucketID), _objID(objID), _serv_content(serv_content)
        {
        }

        virtual void run()
        {
                _s->Get(_bucketID, _objID, _serv_content);
        }


private:
        const FleCS::SM2SPrx& _s;
        const string& _bucketID;
        const string& _objID;
	FleCS::ByteSeq& _serv_content;
};

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


class rep_partial : public Container
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

		/*Get from nearest location*/
		string localhost = boost::asio::ip::host_name();
		
		redisContext *c;
                redisReply *reply;

                struct timeval timeout = { 1, 500000 }; // 1.5 seconds
                c = redisConnectWithTimeout((char*)"flecs52", 6379, timeout);
                if (c->err) {
                        _LOG("Redis Connection error"<<c->errstr);
                        //exit(1);
		}
                _LOG("Redis connected");

                /* Set a key */
                reply = (redisReply*)redisCommand(c, "GET %s",(string(bucketID + ":" + objID+ ":hosts").c_str()));
                _LOG("GET:"<< reply->str);
		string hosts = reply->str;
                freeReplyObject(reply);
		
		string nearest_host;
		if(!hosts.empty())
			nearest_host = getNearestHost(localhost,hosts);
		else
			_LOG("Redis: Key not found for object");
		if(!nearest_host.compare(localhost)){
			reply = (redisReply*)redisCommand(c, "GET %s",(string(bucketID + ":" + objID+ ":location").c_str()));
			_LOG("GET:"<< reply->str);
			string path = reply->str;
			_readfile(reply->str, content);
			freeReplyObject(reply);
		}
		else{
			/*Get from other host*/
			map<string, FleCS::SM2SPrx*>& s = FleCSServer::peer_servers;
			vector<IceUtil::ThreadPtr> tpv;
			vector<IceUtil::ThreadControl> tcv;

			for (map<string, FleCS::SM2SPrx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
			{
				string endpoint = i->first;
				string host = getHostFromEP(endpoint);
				if(!host.compare(nearest_host))
				{
					IceUtil::ThreadPtr t = new GetThread(*(i->second), bucketID, objID, content);
					tcv.push_back(t->start());
					tpv.push_back(t);
				}
			}

			for (vector<IceUtil::ThreadControl>::iterator j = tcv.begin(); j != tcv.end(); ++ j)
				j->join();
		}

		//_readfile((string(FleCSServer::stg_root_dir) + "/" + boID).c_str(), content);
		_LOG("Get done");
	}
	

	void C2S_Put(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content)
	{
		_LOG(objID);

		string boID = bucketID + "/" + objID;

		GlobalLock lock(boID, 'W');


		/*Get Random server to write to */
	/*
	*0->flecs12
	*1->flecs22
	*2->REDIS_HOST 
	*/
		int s1 = getRand();
		int s2=s1;
		while(s2!=s1)
			s2=getRand();

	        string localhost = boost::asio::ip::host_name();
		string server1 = getHostName(s1);
		string server2 = getHostName(s2);
		
		int local = 0;
		if(!server1.compare(localhost))
			local=1;
		else if(!server2.compare(localhost))
			local=1;
		/*Reis Implementation */
		redisContext *c;
		redisReply *reply;

		struct timeval timeout = { 1, 500000 }; // 1.5 seconds
		c = redisConnectWithTimeout((char*)"flecs52", 6379, timeout);
		if (c->err) {
			_LOG("Connection error"<<c->errstr);
			//exit(1);
		 }
		_LOG("Redis connected");
		if(local)
		{
			reply = (redisReply*)redisCommand(
                                c,
                                "GET %",
                                (string(bucketID + ":" + objID + ":" + "hosts").c_str()));
	                string hosts = reply->str;
			freeReplyObject(reply);

			if(!hosts.empty())
				hosts+=","+localhost;
			else 
				hosts = localhost;
			/* Set a key */
			reply = (redisReply*)redisCommand(c, "GET %s",(string( bucketID + ":" + objID + ":" + "hosts").c_str()));
			_LOG("SET:"<< reply->str);
			freeReplyObject(reply);
			
			reply = (redisReply*)redisCommand(
                                        c,
                                        "SET %s %s",
                                        (string(bucketID + ":" + objID + ":" + "location").c_str()),
                                        (string(FleCSServer::stg_root_dir)+ "/" + bucketID + "/" + objID).c_str());
                        _LOG("SET:"<< reply->str);
                        freeReplyObject(reply);

			
			reply = (redisReply*)redisCommand(c, "GET %s",(string( bucketID + ":" + objID + ":" + "hosts").c_str()));
			_LOG("GET:"<< reply->str);

			//_writefile(reply->str, content);
			_writefile((string(FleCSServer::stg_root_dir) + "/" + boID).c_str(), content);
			freeReplyObject(reply);
			// propagate update to the other servers.
		}
		map<string, FleCS::SM2SPrx*>& s = FleCSServer::peer_servers;

#ifdef _SERIAL_PROCESSING
		for (map<string, FleCS::SM2SPrx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
		{
			string endpoint = i->first;
			string host = getHostFromEP(endpoint);
			if((!host.compare(server1))||(!host.compare(server2))){
				(*(i->second))->Put(bucketID, objID, content);
			}
		}

#else	// Parallel processing (by default)
		vector<IceUtil::ThreadPtr> tpv;
		vector<IceUtil::ThreadControl> tcv;

		for (map<string, FleCS::SM2SPrx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
		{
			string endpoint = i->first;
                        string host = getHostFromEP(endpoint);
                        if((!host.compare(server1))||(!host.compare(server2)))
			{
				IceUtil::ThreadPtr t = new PutThread(*(i->second), bucketID, objID, content);
				tcv.push_back(t->start());
				tpv.push_back(t);
			}
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

		string localhost = boost::asio::ip::host_name();
         	redisContext *c;
                redisReply *reply;

                struct timeval timeout = { 1, 500000 }; // 1.5 seconds
                c = redisConnectWithTimeout((char*)"flecs52", 6379, timeout);
                if (c->err) {
                        _LOG("Connection error"<<c->errstr);
                        //exit(1);
                 }
                _LOG("Redis connected");

		reply = (redisReply*)redisCommand(c, "GET %s",(string(bucketID + ":" + objID + ":hosts").c_str()));
		string hosts = reply->str;
	        _LOG("GET:"<< reply->str);
		freeReplyObject(reply);

		string nearest_host;
                if(!hosts.empty())  
                        nearest_host = getNearestHost(localhost,hosts);
                else
                        _LOG("Redis: Key not found for object");
                if(!nearest_host.compare(localhost)){
                        reply = (redisReply*)redisCommand(c, "GET %s",(string(bucketID + ":" + objID+ ":location").c_str()));
                        _LOG("GET:"<< reply->str);
                        string path = reply->str;
                        _appendfile(reply->str, content);
                        freeReplyObject(reply);
                }
		 else{
                        /*Get from other host*/
                        map<string, FleCS::SM2SPrx*>& s = FleCSServer::peer_servers;
                        vector<IceUtil::ThreadPtr> tpv;
                        vector<IceUtil::ThreadControl> tcv;

                        for (map<string, FleCS::SM2SPrx*>::const_iterator i = s.begin(); i != s.end(); ++ i)
                        {
                                string endpoint = i->first;
                                string host = getHostFromEP(endpoint);
				if(hosts.find(host)!=string::npos)
				{
                                	IceUtil::ThreadPtr t = new AppendThread(*(i->second), bucketID, objID, content);
	 	                        tcv.push_back(t->start());
                	                tpv.push_back(t);
				}
                        }

                        for (vector<IceUtil::ThreadControl>::iterator j = tcv.begin(); j != tcv.end(); ++ j)
                                j->join();
                }



		//_appendfile(reply->str, content);

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
	}


	// This function is called with a 'W' lock held.
	void S2S_Put(
		const std::string& bucketID,
		const std::string& objID,
		const FleCS::ByteSeq& content)
	{
		_LOG(objID);
		
		redisContext *c;
                redisReply *reply;

                struct timeval timeout = { 1, 500000 }; // 1.5 seconds
                c = redisConnectWithTimeout((char*)"flecs52", 6379, timeout);
                if (c->err) {
                        _LOG("Connection error"<<c->errstr);
                        //exit(1);
                 }
                _LOG("Redis connected");

		 /*Append to existing hosts*/
		reply = (redisReply*)redisCommand(
				c,
				"GET %",
				(string(bucketID + ":" + objID + ":hosts").c_str()));
		string hosts = reply->str;
		freeReplyObject(reply);

		string localhost = boost::asio::ip::host_name();
		if(!hosts.empty())	
			hosts+=","+localhost;
		else
			hosts=localhost;
		 /* Set a key */
		reply = (redisReply*)redisCommand(
				c,
				"SET %s %s",
				(string(bucketID + ":" + objID + ":" + "hosts").c_str()),
				hosts.c_str());
		_LOG("SET:"<< reply->str);
		freeReplyObject(reply);

		reply = (redisReply*)redisCommand(
				c,
				"SET %s %s",
				(string(bucketID + ":" + objID + ":" + "location").c_str()),
				(string(FleCSServer::stg_root_dir)+ "/" + bucketID + "/" + objID).c_str());
		_LOG("SET:"<< reply->str);
		freeReplyObject(reply);

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

	//This function is called with a 'R' lock held.
	void S2S_Get(
                const std::string& bucketID,
                const std::string& objID,
                FleCS::ByteSeq& content)
        {
                _LOG(objID);
		_readfile((string(FleCSServer::stg_root_dir) + "/" + bucketID + "/" + objID).c_str(), content);
	}

	int getRand()
	{
		srand(time(NULL));
		return rand()%3;
	}
	
	string getHostName(int host_id)
	{
		string host_name;
		switch(host_id)
		{
			case 0:
				host_name = "flecs12";
			case 1:	
				host_name = "flecs22";
			case 2:
				host_name = "flecs52";
		}		
		return host_name;
	}
	int getHostID(string hostname)
	{
		int id = 0;
		
		if(!hostname.compare("flecs12"))
			id = 0;

		else if(!hostname.compare("flecs22"))
			id = 1;

		else if(!hostname.compare("flecs52"))
			id = 2;
		return id;
	}
	string getHostFromEP(string endpoint)
	{
		string host;
		char *ep = new char [endpoint.size()+1]; 
		strcpy(ep, endpoint.c_str());
		char *pch;
		int i = 0;
		pch = strtok (ep," ");
		while(pch!=NULL){
			i++;
			if(i==4)
				host = pch;
			pch = strtok(NULL," ");
        	}   
		return host;
	}
	string getNearestHost(string localhost, string hosts)
	{
		/*
		*Distances
		*flecs12<->flecs22 = 1
		*flecs12<->flecs52 = 2
		*flecs22<->flecs52 = 1
		*flecs22<->flec
		*/
		string server1, server2;
		char* hnames = new char [hosts.size()+1];
		strcpy(hnames, hosts.c_str());
		char *pch;
		pch = strtok(hnames,",");
		server1 = pch;
		pch = strtok(NULL,",");
		server2 = pch;
		int distance1 = abs(getHostID(localhost)-getHostID(server1));
		int distance2 = abs(getHostID(localhost)-getHostID(server2));
		if(distance1<distance2)
			return server1;
		return server2;
	}

private:
};


extern "C" rep_partial* CreateInstance()
{
	return new rep_partial;
}
