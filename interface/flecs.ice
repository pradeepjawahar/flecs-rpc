#ifndef __FLECS_ICE__
#define __FLECS_ICE__


module FleCS
{
	sequence<byte> ByteSeq;
	sequence<string> StringSeq;


	// Client-to-server
	interface C2S
	{
		void Get(string bucketID, string objID, out ByteSeq content);
		void Put(string bucketID, string objID, ByteSeq content);
		void Append(string bucketID, string objID, ByteSeq content);
		void Delete(string bucketID, string objID);
		void Process(string bucketID, string objID);
	};


	// Server-to-server and master-to-server.
	interface SM2S
	{
		// Server to server
		void Get(string bucketID, string objID, out ByteSeq content);
		void Put(string bucketID, string objID, ByteSeq content);
		void Append(string bucketID, string objID, ByteSeq content);
		void Delete(string bucketID, string objID);
		void Process(string bucketID, string objID);

		// Global lock
		void AcquireLock(string lockPath, byte type, string hostname, long tid);
		void ReleaseLock(string lockPath, byte type, string hostname, long tid);


		// Master to server notification
		void ServerJoined(string endpoint);
		void ServerLeft(string endpoint);
	};


	interface Master
	{
		void Join(string endpoint, out StringSeq existingServers);

		StringSeq GetLockServers();
	};
};

#endif
