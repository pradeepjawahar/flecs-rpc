#ifndef __FLECS_ICE__
#define __FLECS_ICE__


module FleCS
{
	sequence<byte> ByteSeq;

	interface C2S
	{
		// TODO: Add version as a parameter. A simple scalar value will do? or vector?

		void Get(string objID, out ByteSeq content);
		void Put(string objID, ByteSeq content);
		void Append(string objID, ByteSeq content);
	};


	interface S2S
	{
		void Append(string objID, ByteSeq content);
	};
};

#endif
