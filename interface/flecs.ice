#ifndef __FLECS_ICE__
#define __FLECS_ICE__


module FleCS
{
	sequence<byte> ByteSeq;
	sequence<string> StringSeq;


	// Client-to-server
	interface C2S
	{
		// TODO: Add version as a parameter. A simple scalar value will do? or vector?
		void Get(string objID, out ByteSeq content);
		void Put(string objID, ByteSeq content);
		void Append(string objID, ByteSeq content);
	};


	// Server-to-server and master-to-server.
	interface Server
	{
		// Server to server
		void Append(string objID, ByteSeq content);

		// Master to server notification
		void ServerJoined(string endpoint);
		void ServerLeft(string endpoint);
	};


	interface Master
	{
		void Join(string endpoint, out StringSeq existingServers);
	};
};

#endif
