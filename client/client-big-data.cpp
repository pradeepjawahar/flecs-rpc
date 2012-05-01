#include <fstream>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <string.h>
#include <sstream>
#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <boost/lexical_cast.hpp>
#include <Ice/Ice.h>


#include <log4cxx/logger.h>

#include "flecs.h"
#include "util.h"
#include "hiredis/hiredis.h"

#define _LOG(A) LOG4CXX_INFO(logger, A)

using namespace std;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("client"));


class FleCSClient : public Ice::Application
{
public:
    FleCSClient() :
		Ice::Application(Ice::NoSignalHandling)
	{
		_hostname = boost::asio::ip::host_name();
	}


	virtual int run(int argc, char* argv[])
	{
		try
		{
			_init();

			FleCS::ByteSeq content;

			//_c2s_prx->Get("rep-partial", "00000001/00000001/00000001", content);

			//_writefile("abc", content);
			//_Put("b/c");

			return EXIT_SUCCESS;
		}
		catch (const exception& e)
		{
			_LOG(e.what());
		}

		return EXIT_FAILURE;
	}


private:
	void _init()
	{
		// init RPC proxy
		_c2s_prx = FleCS::C2SPrx::checkedCast(
				communicator()
				->propertyToProxy("FleCS_c2s.Proxy")
				->ice_twoway()
				->ice_timeout(-1)
				->ice_secure(false));
		if(!_c2s_prx)
		{
			_LOG("invalid proxy");
			exit(EXIT_FAILURE);
		}

		_LOG("Initialized.");
	}
	/*void _Put(const char *objname)
        {
                FleCS::ByteSeq content;

                for (int i = 0; i <1000 ; i ++)
                {
                        content.insert(content.end(), _hostname.c_str(), _hostname.c_str() + _hostname.size());
                        content.push_back('.');
                }
                content.push_back('\n');

                StopWatch sw;
                //TODO@ : Don't hard code bucket and obj name
                _c2s_prx->Put("rep-redis", "b/c", content);
                unsigned int laps = sw.GetMicro();
                _LOG("A " << laps << " " << content.size());

        }*/
	void _Put(const char *objname)
	{
	/*	int parts = 0;
		_splitfile(objname,nChunkSize,parts);
		if(parts==0){
			_LOG("Error splitting file");
		}
			
		else{
			_LOG("File split successful");
		}
		FleCS::ByteSeq content;	
		//Write the no.of chunk to server
		string objid = objname;
		string npart = boost::lexical_cast<string>(parts);
		
		content.insert(content.end(),npart.c_str(), npart.c_str() + npart.size());
		_c2s_prx->Put("rep-partial",(string(objid + "part")).c_str(),content);
		content.clear();
		//Write the chunks	*/
	}
	void _Split(const char *objname)
	{
		//default chunk size 64 MB
		int nChunkSize = 67108864;
		std::ostringstream sStringer;
    		ifstream::pos_type nSize;
		ifstream fSource(objname, ios_base::ate|ios::binary|ios::in);
		char * sMemBlock;
		nSize = fSource.tellg();
		string sExtension = strstr(objname,".");
		unsigned long nGetPointer = 0;
		int nLastChunkSize = nSize%nChunkSize;
		int nPartNumber = 1;
		string sDestinationFile;
		unsigned long currSize;
		sMemBlock = new char[nChunkSize];
		FleCS::ByteSeq content;
		if (fSource.is_open())
		{
			fSource.seekg(0, ios::beg);

			while (fSource)
			{
			    fSource.seekg(nGetPointer, ios::beg);
			    currSize = nGetPointer + nChunkSize;
			    if (currSize > (unsigned int)nSize)
			    {
				nChunkSize = nLastChunkSize;
				sMemBlock = new char[nChunkSize];
				fSource.read(sMemBlock, nChunkSize);
				content.insert(content.end(),sMemBlock,sMemBlock + nChunkSize);
				string objid = objname;
				objid+=".part";
				_c2s_prx->Put("rep-partial",(string(objname + (boost::lexical_cast<string>(nPartNumber)))).c_str(),content);
				content.clear();

			    }
			    else
				    {
				fSource.read(sMemBlock, nChunkSize);
				content.insert(content.end(),sMemBlock,sMemBlock + nChunkSize);
				content.clear();
			    }
			    nGetPointer += nChunkSize;
			    nPartNumber += 1;
			}
		}
		delete [] sMemBlock;
		    //Write chunks to redis
	        nPartNumber--;
	        string npart = boost::lexical_cast<string>(nPartNumber);
	        content.insert(content.end(),npart.c_str(), npart.c_str() + npart.size());
	        _c2s_prx->Put("rep-partial",(string(objname) + "partno").c_str(),content);
	        content.clear();	
	}
	void _Merge(string objid);
	void _Get_Part(const char *objname ,int offset)
	{
		FleCS::ByteSeq content;
		int chunksize = 67108864;
		int part = offset/chunksize;
		//get this part from server
		string objid = objname;
                objid+=".part";
                _c2s_prx->Get("rep-partial",(string(objname + (boost::lexical_cast<string>(part)))).c_str(),content);

	}
	
	int offset_to_part(int offset);
		
	FleCS::C2SPrx _c2s_prx;
	string _hostname;
};


int main(int argc, char* argv[])
{
	int rc = EXIT_FAILURE;

	cout << "client started.\n";

	if (daemon(1, 0) == -1)
	{
		perror("daemon");
		return EXIT_FAILURE;
	}

	_LOG("Client starting...");

	try
	{
		FleCSClient app;
		rc = app.main(argc, argv, "config.client");
	}
	catch(const exception& e)
	{
		_LOG(e.what());
	}

	_LOG("Client finished.");

	return rc;
}
