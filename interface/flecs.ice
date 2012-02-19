#ifndef __FLECS_ICE__
#define __FLECS_ICE__


module FleCS
{
	sequence<byte> ByteSeq;


	// Client-to-server
	interface C2S
	{
		// TODO: Add version as a parameter. A simple scalar value will do? or vector?
		void Get(string objID, out ByteSeq content);
		void Put(string objID, ByteSeq content);
		void Append(string objID, ByteSeq content);
	};


	// Server-to-server and master-to-server.
	// It seems that I can't user the name "Server".
	interface Server1
	{
		// Server to server
		void Append(string objID, ByteSeq content);

		// Master to server notification
		void ServerAdded(string endpoint);
		void ServerRemoved(string endpoint);
	};


	interface Master
	{
		void ServerReady(string endpoint);
	};
};

#endif
